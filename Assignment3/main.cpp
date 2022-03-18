// Ricardo Morales Gonzalez rmorale5  Ana Kuri
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <map>
using namespace std;



struct Slot {
  //tag = left overs/thing to check
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
  unsigned filled_slots;
  // unsigned is tag, and points to whatever slot
  map<unsigned, unsigned> directory;
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
  unsigned store_hits;
  unsigned store_misses;
  unsigned total_cycles;
};

void final_print(Cache* info) {
  cout << "Total loads: " << info->total_loads << endl;
  cout << "Total stores: " << info->total_stores << endl;
  cout << "Load hits: " << info->load_hits << endl;
  cout << "Load misses: " << info->load_misses << endl;
  cout << "Store hits: " << info->store_hits << endl;
  cout << "Store misses: " << info->store_misses << endl;
  cout << "Total cycles: " << info->total_cycles << endl;
}

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


int invalids(int num_sets, int block_size, int num_block_bytes, string write_hit, string write_miss, string eviction_type, string set, string block, string block_bytes) {
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
  return 0;
}

void cache_setup(Cache* mainCache, int num_sets, int block_size, int num_block_bytes) {
  //set up the cache
  mainCache->num_sets = num_sets;
  mainCache->num_blocks_per_set = block_size;
  mainCache->container_size = num_block_bytes;
  mainCache->total_loads = 0;
  mainCache->total_stores = 0;
  mainCache->load_hits = 0;
  mainCache->load_misses = 0;
  mainCache->store_hits = 0;
  mainCache->store_misses = 0;
  mainCache->total_cycles = 0;


  
  for (int i = 0; i < num_sets; i++) {
    Set simple_set;
    simple_set.filled_slots = 0;
    for (int j = 0; j < block_size; j++) {
      Slot simple_slot;
      simple_slot.tag = 0;
      simple_slot.valid = false;
      simple_slot.load_ts = 0;
      simple_slot.access_ts = 0;
      simple_slot.dirty = false;
      simple_set.slots.push_back(simple_slot);
    }
    
    mainCache->sets.push_back(simple_set);
    
  }
  
}

void lruFinish(Cache* mainCache, unsigned index, unsigned slot_index){

  for (unsigned i = 0; i < mainCache->num_blocks_per_set; i++) {
    if (mainCache->sets[index].slots[i].valid == true && mainCache->sets[index].slots[i].access_ts != mainCache->sets[index].slots[slot_index].access_ts) {
      mainCache->sets[index].slots[i].access_ts++;
    }
  }

  /*
  map<unsigned, unsigned>::iterator itr;
  for (auto itr = mainCache->sets[index].directory.begin(); itr != mainCache->sets[index].directory.end(); itr++) {
    if (mainCache->sets[index].slots[itr->second].access_ts < mainCache->sets[index].slots[slot_index].access_ts) {
      mainCache->sets[index].slots[itr->second].access_ts++;
    }
  }
  */
  mainCache->sets[index].slots[slot_index].access_ts = 0;
}


void lruLoadHit(Cache* mainCache, unsigned index, unsigned slot_index) {
  mainCache->total_cycles++;
  lruFinish(mainCache, index, slot_index);
}

void lruLoadMiss(Cache* mainCache, unsigned index, unsigned tag, unsigned slot_index) {
  unsigned highest_access_ts = 0;
  unsigned index_with_highest = 0;
  bool found_empty = false;
  
  for (unsigned i = 0; i < mainCache->num_blocks_per_set; i++) {
    if (mainCache->sets[index].slots[i].valid == true) {
      if (mainCache->sets[index].slots[i].access_ts > highest_access_ts) {
	highest_access_ts = mainCache->sets[index].slots[i].access_ts;
	index_with_highest = i;
      }
    } else {
      found_empty = true;
      index_with_highest = i;
      break;
    }
  }

  slot_index = index_with_highest;
  if (found_empty == false) {
    mainCache->sets[index].directory.erase(mainCache->sets[index].slots[slot_index].tag);
  }
  if (mainCache->sets[index].slots[slot_index].dirty == true) {
    mainCache->total_cycles += (100 * ((mainCache->container_size) / 4));
    mainCache->sets[index].slots[slot_index].dirty = false;
  }
  
  mainCache->sets[index].slots[slot_index].valid = true;
  mainCache->sets[index].slots[slot_index].tag = tag;
  
  mainCache->sets[index].directory.insert({tag, slot_index});
  lruFinish(mainCache, index, slot_index);
}



void storeMiss(Cache* mainCache, unsigned index, unsigned tag, unsigned slot_index, string write_miss) {
  
	
  //write-allocate: load into cache, update line in cache
  if (write_miss == "write-allocate") {
    mainCache->total_cycles++;
    lruLoadMiss(mainCache, index, tag, slot_index);
    
  }
  
  //no-write-allocate: writes straight to memory, does not load into cache
  if (write_miss == "no-write-allocate") {
    mainCache->total_cycles += 1 + (100 * ((mainCache->container_size) / 4));
  }
  
}


void lruStoreHit(Cache* mainCache, unsigned index, unsigned tag, string write_hit, unsigned slot_index) {
	  
  //write-through: write immediately to memory
  if (write_hit == "write-through") {
    
    mainCache->sets[index].slots[slot_index].tag = tag;
    mainCache->total_cycles += 1 + (100 * ((mainCache->container_size) / 4));
  }
  //write-back: defer to write to memory until replacement of line
  if (write_hit == "write-back") {
    mainCache->sets[index].slots[slot_index].dirty = true;
  }
  
  lruFinish(mainCache, index, slot_index);
  
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

  if (invalids(num_sets, block_size, num_block_bytes, write_hit, write_miss, eviction_type, set, block, block_bytes) == 1) {
    return 1;
  }
  Cache mainCache;
  cache_setup(&mainCache, num_sets, block_size, num_block_bytes);
  
  //unsigned time = 0;
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
    
    //convert binary tag and index to decima
    unsigned tag = stoi(tag_str, 0, 2);
    unsigned index = 0;
    if (index_str.length() > 0) {
      index = stoi(index_str, 0, 2);
    }

    unsigned slot_index;
    int key_count = -1;
    for (unsigned i = 0; i < mainCache.num_blocks_per_set; i++) {
      if (mainCache.sets[index].slots[i].tag == tag) {
	slot_index = i;
	key_count = 1;
      }
    }

    
    //int key_count = mainCache.sets[index].directory.count(tag);
    //unsigned slot_index;
    
    //loading
    if (load_store == 'l') {

      //load miss
      if (key_count <= 0) {
	mainCache.load_misses++;
	mainCache.total_cycles += 1 + (100 * (mainCache.container_size) / 4);
	
	lruLoadMiss(&mainCache, index, tag, slot_index);
	
	//load---- hit
      } else {
	mainCache.load_hits++;
	slot_index = mainCache.sets[index].directory[tag];
	lruLoadHit(&mainCache, index, slot_index);
	
      }

      mainCache.total_loads++;
      
      //store
    } else {
      
      //store miss
      if (key_count <= 0) {
	mainCache.store_misses++;
        storeMiss(&mainCache, index, tag, slot_index, write_miss);
	//store-hit
      } else {
	mainCache.store_hits++;
	slot_index = mainCache.sets[index].directory[tag];
	lruStoreHit(&mainCache, index, tag, write_hit, slot_index);
	
      }
      mainCache.total_stores++;
      
    }
    
  }
  final_print(&mainCache);
  return 0;
}
