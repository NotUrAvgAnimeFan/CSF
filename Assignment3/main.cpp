
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;



struct Slot {
  //tag = left overs/where it came from
  //index = which set
  unsigned tag;
  bool valid;
  //load_ts for FIFO
  //access_ts for LRU
  unsigned load_ts, access_ts;
  bool dirty;
};

struct Set {
  vector<Slot> slots;
  map<unsigned, Slot*> index;
};

struct Cache {
  vector<Set> sets;
  
  unsigned num_sets;
  unsigned num_blocks_per_set;
  unsigned container_size;
  
  unsigned total_loads;
  unsigned total_stores;
  unsigned load_hits;
  unsigned load_misses;
  unsigned hits;
  unsigned misses;
  unsigned total_cycles;
};


string hexToBinary(string hexadecimal) {
  string binary;

  int i = 0;
  while (hexadecimal[i]) {
    switch(hexadecimal[i]) {
    case '0':
      binary.append("0000");
      break;
    case '1':
      binary.append("0001");
      break;
    case '2':
      binary.append("0010");
      break;
    case '3':
      binary.append("0011");
      break;
    case '4':
      binary.append("0100");
      break;
    case '5':
      binary.append("0101");
      break;
    case '6':
      binary.append("0110");
      break;
    case '7':
      binary.append("0111");
      break;
    case '8':
      binary.append("1000");
      break;
    case '9':
      binary.append("1001");
      break;
    case 'a':
      binary.append("1010");
      break;
    case 'b':
      binary.append("1011");
      break;
    case 'c':
      binary.append("1100");
      break;
    case 'd':
      binary.append("1101");
      break;
    case 'e':
      binary.append("1110");
      break;
    case 'f':
      binary.append("1111");
      break;
    }
    i++;
  }

  return binary;
}



int main(int argc, char* argv[]) {

  //print an error if invalid number of parameters
  if (argc != 7) {
    cerr << "error: invalid number of parameters";
    return 1;
  }
  int num_sets = atoi(argv[1]);
  int block_size = atoi(argv[2]);
  int num_block_bytes = atoi(argv[3]);
  string write_miss = argv[4];
  string write_hit = argv[5];
  string eviction_type = argv[6];


  //if any cache parameters are invalid, print and error message and exit
  string set = argv[1];
  string block = argv[2];
  string block_bytes = argv[3];

  if (set.find_first_not_of("0123456789") != string::npos || block.find_first_not_of("0123456789") != string::npos || block_bytes.find_first_not_of("0123456789") != string::npos) {
    cerr << "error: invalid integer" << endl;
    return 1;
  }
  
  if ((num_sets & (num_sets - 1)) != 0 || num_sets == 0) {
    cerr << "error: number of sets isn't a power of 2" << endl;
    return 1;
  }
  if (block_size == 0 || (block_size & (block_size - 1)) != 0) {
    cerr << "error: block size isn't a power of 2" << endl;
    return 1;
  }
  if (num_block_bytes == 0 || (num_block_bytes & (num_block_bytes - 1)) != 0) {
    cerr << "error: number of bytes in each block isn't a power of 2" << endl;
    return 1;
  }
  if (num_block_bytes < 4) {
    cerr << "error: block size is less than 4" <<endl;
    return 1;
  }
  if (write_hit.compare("write-back") == 0 && write_miss.compare("no-write-allocate") == 0) {
    cerr << "error: write-back and no-write-allocate both specified" << endl;
    return 1;
  }

  if (write_hit.compare("write-back") != 0 && write_hit.compare("write-through") != 0) {
    cerr << "error: neither write-back nor write-through specified" << endl;
    return 1;
  }
  if (write_miss.compare("write-allocate") != 0 && write_miss.compare("no-write-allocate") != 0) {
    cerr << "error: neither write-allocate nor no-write-alllocate specified" << endl;
    return 1;
  }
  if (eviction_type.compare("fifo") != 0 && eviction_type.compare("lru") != 0) {
    cerr << "error: neither fifo nor lru specified" << endl;
    return 1;
  }

  //set up the cache
  Cache mainCache;
  mainCache.num_sets = num_sets;
  mainCache.num_blocks_per_set = block_size;
  mainCache.container_size = num_block_bytes;
  
  for (int i = 0; i < (int)mainCache.num_sets; i++) {
    Set simple_set;
    for (int j = 0; j < (int)mainCache.num_blocks_per_set; j++) {
      Slot simple_slot;
      simple_slot.tag = 0;
      simple_slot.valid = false;
      simple_slot.load_ts = 0;
      simple_slot.access_ts = 0;
      simple_set.slots.push_back(simple_slot);
    }
    mainCache.sets.push_back(simple_set);
    
  }

  unsigned time = 0;
  char load_store;
  string address;
  string third_var;
  string line;

  while (getline(cin,line)) {
    //store each memory access field
    stringstream ss;
    ss << line;
    ss >> load_store >> address >> third_var;
    
    // hits happen when found data in cache
    // miss when data not found in cache
    // both for loading
    // when miss reques from main memory, send data to cache, store in cache, send to CPU

    string binary_form = hexToBinary(address.substr(2, 8));
    
    // slots = 2^num_of_index_bits
    // block size = 2^offset bits
    // capacity = slots * block size
    int total_cache_capacity = log2(mainCache.num_sets * mainCache.container_size);
    int num_offset_digits = log2(mainCache.container_size);
    int num_index_digits = total_cache_capacity - num_offset_digits;
    int num_tag_digits = 32 - num_offset_digits - num_index_digits;
    string tag_str = binary_form.substr(0, num_tag_digits);
    string index_str = binary_form.substr(num_tag_digits, num_index_digits);
    
    //convert binary tag and index to decimal
    unsigned tag = stoi(tag_str, 0, 2);
    unsigned index = stoi(index_str, 0, 2);
    
    //loading
    if (load_store == 'l') {
      bool hit = false;
      bool all_used = true;
      int i;
      for (i = 0; i < mainCache.num_blocks_per_set; i++) {
	if (mainCache.sets[index].slots[i].valid == true) {
	  if (mainCache.sets[index].slots[i].tag == tag) {
	    hit = true;
	  }
	} else {
	  all_used = false;
	}
      }

      // a miss if data not found in the cache
      if (hit == false) {
	mainCache.misses++;
	maincache.load_misses++;
	mainCache.total_cycles += 1 + (100 * (mainCache.container_size) / 4);

	if (all_used == true && eviction_type = "LRU") {
	  int index_last_used = 0;
	  int highest_num = mainCache.sets[index].slots[0].access_ts;
	  
	  for (i = 1; i < mainCache.num_blocks_per_set; i++) {
	    if (mainCache.sets[index].slots[i].access_ts > highest_num) {
	      index_last_used = i;
	      highest_num = mainCache.sets[index].slots[i].access_ts;
	    }
	  }
	  mainCache.sets[index].slots[index_last_used].tag = tag;
	  mainCache.sets[index].slots[index_last_used].access_ts = 0;

	  for (i = 0; i < mainCache.num_blocks_per_set; i++) {
	    if (i != index_last_used) {
	      mainCache.sets[index].slots[index_last_used].access_ts++;
	    }
	  }
	  
	  
	}
	mainCache.sets[index].slots[i].tag == tag;
      } else {
	
      }
      
      
      //a miss if data not found in the cache
      if (mainCache.sets[index].slots[0].valid == false || mainCache.sets[index].slots[0].tag != tag) {
	mainCache.misses++;
	mainCache.load_misses++;
	mainCache.total_cycles += 1 + (100 * (mainCache.container_size / 4));
	mainCache.sets[index].slots[0].tag = tag;
	
	//a hit if data found in the cache
      } else {
	mainCache.hits++;
	mainCache.load_hits++;
	mainCache.total_cycles++;
	
      }
      mainCache.total_loads++;
      mainCache.sets[index].slots[0].access_ts = time;
    }
    
    //storing
    else {
      //a hit if data found in the cache
      if (mainCache.sets[index].slots[0].valid == true && mainCache.sets[index].slots[0].tag == tag) {
	//write-through
	
	//write-back
	
	//a miss if data not found in the cache
      } else {
	//write-allocate
	
	//no-write-allocate
	
      }
      
    }
    
  }
  
  
  
  /*
  // direct mapping
  if (mainCache.num_blocks_per_set == 1) {
    while (getline(cin,line)) {
      //store each memory access field
      stringstream ss;
      ss << line;
      ss >> load_store >> address >> third_var;
      
      // hits happen when found data in cache
      // miss when data not found in cache
      // both for loading
      // when miss reques from main memory, send data to cache, store in cache, send to CPU

      //loading
      if (load_store == 'l') {
	
	string binary_form = hexToBinary(address.substr(2, 8));
	
	// slots = 2^num_of_index_bits
	// block size = 2^offset bits
	// capacity = slots * block size
	int total_cache_capacity = log2(mainCache.num_sets * mainCache.container_size);
	int num_offset_digits = log2(mainCache.container_size);
	int num_index_digits = total_cache_capacity - num_offset_digits;
	int num_tag_digits = 32 - num_offset_digits - num_index_digits;
	string tag_str = binary_form.substr(0, num_tag_digits);
	string index_str = binary_form.substr(num_tag_digits, num_index_digits);

	//convert binary tag and index to decimal
	unsigned tag = stoi(tag_str, 0, 2);
	unsigned index = stoi(index_str, 0, 2);
	
	//a miss if data not found in the cache
	if (mainCache.sets[index].slots[0].valid == false || mainCache.sets[index].slots[0].tag != tag) {
	  mainCache.misses++;
	  mainCache.load_misses++;
	  mainCache.total_cycles += 1 + (100 * (mainCache.container_size / 4));
	  mainCache.sets[index].slots[0].tag = tag;

	//a hit if data found in the cache
	} else {
	  mainCache.hits++;
	  mainCache.load_hits++;
	  mainCache.total_cycles++;
    
	}
	mainCache.total_loads++;
	mainCache.sets[index].slots[0].access_ts = time;
      }

      //storing
      else {
	//a hit if data found in the cache
	if (mainCache.sets[index].slots[0].valid == true && mainCache.sets[index].slots[0].tag == tag) {
	  //write-through

	  //write-back
	
	//a miss if data not found in the cache
	} else {
	  //write-allocate

	  //no-write-allocate

	}
	
      }
      
    }
    
  } else if (mainCache.num_sets == 1) {

  } else {

  }  

  */
  
  return 0;
}
