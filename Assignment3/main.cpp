
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;



struct Slot {
  unsigned tag;
  unsigned index;
  unsigned offset;
  bool valid;
  unsigned load_ts, access_ts;
  bool dirty;
};

struct Set {
  vector<Slot> slots;
};

struct Cache {
  vector<Set> sets;
  
  unsigned num_sets;
  unsigned block_size;
  unsigned container_size;
  
  unsigned total_loads;
  unsigned total_stores;
  unsigned load_hits;
  unsigned load_misses;
  unsigned hits;
  unsigned misses;
  unsigned total_cycles;
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
  
  //reading traces
  char load_store;
  string address;
  string third_var;
  string line;

  Cache mainCache;
  mainCache.num_sets = num_sets;
  mainCache.block_size = block_size;
  mainCache.container_size = num_block_bytes;

  for (int i = 0; i < mainCache.num_sets; i++) {
    Set simple_set;
    for (int j = 0; j < mainCache.block_size; j++) {
      Slot simple_slot;
      simple_set.push_back(simple_slot);
    }
    mainCache.sets.push_back(simiple_set);
    
  }

  unsigned time = 0;
  if (mainCache.block_size == 1) {
    unsigned indexes_used = 0;
    while (getline(cin,line)) {
      //store each memory access field
      stringstream ss;
      ss << line;
      ss >> load_store >> address >> third_var;
      
      // hits happen when found data in cache
      // miss when data not found in cache
      // both for loading
      // when miss reques from main memory, send data to cache, store in cache, send to CPU
      if (load_store == 'l') {
	// slots = 2^num_of_index_bits
	// block size = 2^offset bits
	// capacity = slots * block size
	int num_index_bits = (log(mainCache.num_sets) / log(2)) / 4;
	int num_tag_bits = (32 - num_index_bits) / 4;
	string tag_str = address.substr(0, num_tag_bits);
	string index_str = address.substr(num_tag_bits, num_index_bits);
	int tag_size = tag_str.size();
	int index_size = index_str.size();

	usigned tag;
	unsigned index;
	
	for (int i = 0; i < tag_size; i++) {
	  if (tag_str[i] >= 'a') {
	    tag += tag_str[i] - 'a' + 10;
	  } else {
	    tag += tag_str[i] - '0';
	  }
	  tag = tag << 4;
	}
	
	for (int i = 0; i < index_size; i++) {
	  if (index_str[i] >= 'a') {
	    index += index_str[i] - 'a' + 10;
	  } else {
	    index += index_str[i] - '0';
	  }
	  index = index << 4;
	}

	for (int i = 0; i <indexes_used; i++) {
	  
	}
	
      }
      else {
	
      }
      
    }
    
  } else if (mainCache.num_sets == 1) {

  } else {

  }  
  
  
  return 0;
}
