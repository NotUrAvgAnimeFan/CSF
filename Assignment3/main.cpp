/*
 * Cache Simulation Program
 * CSF Assignment 3 Milestone 3
 * Ricardo Morales Gonzalez  rmorale5@jhu.edu
 * Ana Kuri  akuri1@jhu.edu
 */
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


/*
 * Prints all necessary information in the correct format
 * 
 * Parameters:
 *   info - a pointer to a Cache object containing all info about cache
 *
 * Returns:
 *   nothing
 */
void final_print(Cache* info) {
  cout << "Total loads: " << info->total_loads << endl;
  cout << "Total stores: " << info->total_stores << endl;
  cout << "Load hits: " << info->load_hits << endl;
  cout << "Load misses: " << info->load_misses << endl;
  cout << "Store hits: " << info->store_hits << endl;
  cout << "Store misses: " << info->store_misses << endl;
  cout << "Total cycles: " << info->total_cycles << endl;
}

/*
 * Converts a char letter into its appropriate binary representation
 * works for numbers 0-9
 * 
 * Parameters:
 *   full_string - a string pointers where the appending of the string will be held
 *   letter - the hexadecimal letter to convert
 *
 * Returns:
 *   nothing
 */
void hexToBinaryNumbers(string* full_string, char letter) {
  if (letter == '0') {
    full_string->append("0000");
  } else if (letter == '1') {
    full_string->append("0001");
  } else if (letter == '2') {
    full_string->append("0010");
  } else if (letter == '3') {
    full_string->append("0011");
  } else if (letter == '4') {
    full_string->append("0100");
  } else if (letter == '5') {
    full_string->append("0101");
  } else if (letter == '6') {
    full_string->append("0110");
  } else if (letter == '7') {
    full_string->append("0111");
  } else if (letter == '8') {
    full_string->append("1000");
  } else if (letter == '9') {
    full_string->append("1001");
  }
}

/*
 * Converts a char letter into its appropriate binary representation
 * works for letters a-f
 * 
 * Parameters:
 *   full_string - a string pointer pointing to the string where the binary
 *     representation will be appended
 *   letter - the hexadecimal char to convert
 *
 * Returns:
 *   nothing
 */
void hexToBinaryLetters(string* full_string, char letter) {
  switch (letter) {
  case 'a':
    full_string->append("1010");
    break;
  case 'b':
    full_string->append("1011");
    break;
  case 'c':
    full_string->append("1100");
    break;
  case 'd':
    full_string->append("1101");
    break;
  case 'e':
    full_string->append("1110");
    break;
  case 'f':
    full_string->append("1111");
    break;
  }
  
}

/*
 * Converts a given string of hexadecimals into an equivalent string
 * in binary
 * 
 * Parameters:
 *   hexadecimal - the hexadecimal string to convert
 *
 * Returns:
 *   a string with a binary representation of the given hexadecimal
 */
string hexToBinary(string hexadecimal) {
  string binary;

  int i = 0;
  while (hexadecimal[i]) {
    if (hexadecimal[i] >= '0' && hexadecimal[i] <= '9') {
      hexToBinaryNumbers(&binary, hexadecimal[i]);
    } else if (hexadecimal[i] >= 'a' && hexadecimal[i] <= 'f') {
      hexToBinaryLetters(&binary, hexadecimal[i]);
    }
    i++;
  }

  return binary;
}

/*
 * Outputs an error if certain invalid characteristics are met
 * 
 * Parameters:
 *   num_sets - the number of sets in the current cache
 *   block_size - the amount of blocks in all sets in the current cache
 *   num_block_bites - the size of an individual block in cache
 *   write_hit - string containing write-hit behavior defined by user
 *   write_miss - string containing write-miss behavior defined by user
 *   eviction_type - string containing eviction_type specified by user
 *   set - string representation of number of sets in cache
 *   block - string representation of number of blocks per set in cache
 *   block_bytes - string representation of number of bytes per block in cache
 *
 * Returns:
 *   a 1 to indicate main to return 1 in case an invalid combination of
 *    parameters is triggered 
 */
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


/*
 * Fills a particular set with the desired number of blocks
 * 
 * Parameters:
 *   simple_set - a pointer to the set in which all slots will be placed
 *   block_size - an int indicating how many slots to put in the set
 *   order - the access_ts order that will be given to each block
 *
 * Returns:
 *   nothing
 */
void setup_slots(Set* simple_set, int block_size, int order) {
  for (int j = 0; j < block_size; j++) {
      Slot simple_slot;
      simple_slot.tag = 0;
      simple_slot.valid = false;
      simple_slot.load_ts = 0;
      simple_slot.access_ts = order--;
      simple_slot.dirty = false;
      simple_set->slots.push_back(simple_slot);
    }
}

/*
 * Sets up a cache to a default state
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to setup
 *   num_sets - an int indicating how many sets will be in the cache
 *   block_size - an int indicating how many blocks there will be per set
 *   num_block_bytes - an int indicating how many bytes there will be per block
 *
 * Returns:
 *   nothing
 */
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
    int order = block_size - 1;
    setup_slots(&simple_set, block_size, order);
    mainCache->sets.push_back(simple_set);
    
  }
  
}


/*
 * Increases the access times of all blocks and sets the desired block 
 *  access time to 0
 * 
 * Parameters:
 *   mainCache - a pointer to the cache with all the info to be changed
 *   index - an unsigned indicating the index of the slots to increase access_ts
 *   slot_index - an int indicating the slot to be changed to 0
 * 
 * Returns:
 *   nothing
 */
void lruFinish(Cache* mainCache, unsigned index, unsigned slot_index){
  //update access times
  
  for (unsigned i = 0; i < mainCache->num_blocks_per_set; i++) {
    if (mainCache->sets[index].slots[i].tag != mainCache->sets[index].slots[slot_index].tag){
      mainCache->sets[index].slots[i].access_ts++;
    }
  }
  
  mainCache->sets[index].slots[slot_index].access_ts = 0;
}


/*
 * Performs operations that happen in a load hit for LRU
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   slot_index - an unsigned indicating the specific slot to affect
 *
 * Returns:
 *   nothing
 */
void lruLoadHit(Cache* mainCache, unsigned index, unsigned slot_index) {
  mainCache->total_cycles++;
  lruFinish(mainCache, index, slot_index);
}

/*
 * Performs operations that happen in a load hit for FIFO
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *
 * Returns:
 *   nothing
 */
void fifoLoadHit(Cache* mainCache) {
  mainCache->total_cycles++;
}

/*
 * Performs operations that happen in a load miss for LRU implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   found_empty - marks the default state of found_empty
 *   slot_to_use - an unsigned indicating the specific slot to affect
 *   highest_ts - an unsigned which will contain the highest last used ts
 *
 * Returns:
 *   an unsigned indicating the slot that now contains the desired tag
 */
unsigned loadMissWork(Cache* mainCache, unsigned index, bool found_empty, unsigned slot_to_use, unsigned highest_ts) {  
  
  //find an empty slot in the set
  for (unsigned i = 0; i < mainCache->num_blocks_per_set; i++) {
    if (mainCache->sets[index].slots[i].valid == false) {
      slot_to_use = i;
      found_empty = true;
    }
    //if the timestamp is bigger, then it's the least recently used index
    if (found_empty == false && mainCache->sets[index].slots[i].access_ts > highest_ts) {
      highest_ts = mainCache->sets[index].slots[i].access_ts;
      slot_to_use = i;
    }
    mainCache->sets[index].slots[i].access_ts++;
  }
  
  //if all slots filled, evict the least recently used and put the new tag
  if (found_empty == false) {
    
    //if block being evicted is dirty, write back to memory
    if (mainCache->sets[index].slots[slot_to_use].dirty == true) {
      mainCache->total_cycles += 1 + 100 * (mainCache->container_size / 4);
    }
    
    mainCache->sets[index].directory.erase(mainCache->sets[index].slots[slot_to_use].tag);
    mainCache->sets[index].slots[slot_to_use].dirty = false;
    
  }
  return slot_to_use;
}


/*
 * Performs operations that happen in a load miss for LRU implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned indicating the tag that is to be added to the cache
 *
 * Returns:
 *   nothing
 */
void lruLoadMiss(Cache* mainCache, unsigned index, unsigned tag) {
  mainCache->total_cycles += 1 + (100 * (mainCache->container_size / 4));
  unsigned slot_to_use = loadMissWork(mainCache, index, false, 0, 0);

  //there is an empty slot
  mainCache->sets[index].directory.insert({tag, slot_to_use});
  mainCache->sets[index].slots[slot_to_use].tag = tag;
  mainCache->sets[index].slots[slot_to_use].valid = true;
  mainCache->sets[index].slots[slot_to_use].access_ts = 0;
}



/*
 * Performs operations that happen in a load miss for FIFO implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   found_empty - marks the default state of found_empty
 *   slot_to_use - an unsigned indicating the specific slot to affect
 *   lowest_creation_time - an unsigned which will contain the lowest load_ts time
 *
 * Returns:
 *   an unsigned indicating the slot that now contains the desired tag
 */
unsigned loadMissFifoWork(Cache* mainCache, unsigned index, bool found_empty, unsigned slot_to_use, unsigned lowest_creation_time) {
  
  if (mainCache->sets[index].slots[0].valid == false) {
    slot_to_use = 0;
    found_empty = true;
  } else  {
    lowest_creation_time = mainCache->sets[index].slots[0].load_ts;
    slot_to_use = 0;
    
    
    //find an empty slot in the set
    for (unsigned i = 1; i < mainCache->num_blocks_per_set; i++) {
      if (mainCache->sets[index].slots[i].valid == false) {
	slot_to_use = i;
	found_empty = true;
	break;
      } else if (found_empty == false && mainCache->sets[index].slots[i].load_ts < lowest_creation_time) {
	//if the timestamp is bigger, then it's the least recently used index
	lowest_creation_time = mainCache->sets[index].slots[i].load_ts;
	slot_to_use = i;
      }
    }
  }
  
  //if all slots filled, evict the least recently used and put the new tag
  if (found_empty == false) {
    
    //if block being evicted is dirty, write back to memory
    if (mainCache->sets[index].slots[slot_to_use].dirty == true) {
      mainCache->total_cycles += 100 * (mainCache->container_size / 4);
    }
    
    mainCache->sets[index].directory.erase(mainCache->sets[index].slots[slot_to_use].tag);
    mainCache->sets[index].slots[slot_to_use].dirty = false;
    
  }
  
  return slot_to_use;
}



/*
 * Performs operations that happen in a load miss for FIFO implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned indicating the tag that is to be added to the cache
 *   creation_time - an unsigned that indicates when the new item in cache was made
 *
 * Returns:
 *   nothing
 */
void fifoLoadMiss(Cache* mainCache, unsigned index, unsigned tag, unsigned creation_time) {
  mainCache->total_cycles += 1 + (100 * (mainCache->container_size / 4));
  unsigned slot_to_use = loadMissFifoWork(mainCache, index, false, 0, 0);

  mainCache->sets[index].directory.insert({tag, slot_to_use});
  mainCache->sets[index].slots[slot_to_use].tag = tag;
  mainCache->sets[index].slots[slot_to_use].valid = true;
  mainCache->sets[index].slots[slot_to_use].load_ts = creation_time;
}

/*
 * Performs operations that happen in a store miss depending on parameters for LRU implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned that was not found in cache
 *   write_miss - a string containing the behavior to perform when a
 *    store miss happens
 *   write_hit - a string containing the behavior in case of store hit
 *
 * Returns:
 *   nothing
 */
void storeMiss(Cache* mainCache, unsigned index, unsigned tag,  string write_miss, string write_hit) {
  
	
  //write-allocate: load into cache, update line in cache
  if (write_miss == "write-allocate") {

    lruLoadMiss(mainCache, index, tag);

    if (write_hit == "write-through") {
      mainCache->total_cycles += 1 + 100;
    }
    
  }
  
  //no-write-allocate: writes straight to memory, does not load into cache
  if (write_miss == "no-write-allocate") {
    mainCache->total_cycles += 100;
  }
  
}


/*
 * Performs operations that happen in a store miss depending on parameters for FIFO implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned that was not found in cache
 *   write_miss - a string containing the behavior to perform when a
 *    store miss happens
 *   write_hit - a string containing the behavior to perform for store hit
 *   creation_time - an unsigned integer indicating when the new item in cache will be created
 *
 * Returns:
 *   nothing
 */
void fifoStoreMiss(Cache* mainCache, unsigned index, unsigned tag, string write_miss, string write_hit, unsigned creation_time) {
  //write-allocate: load into cache, update line in cache
  if (write_miss == "write-allocate") {
    if (write_hit == "write-through") {
      mainCache->total_cycles += 1 + 100;
    }
    
    fifoLoadMiss(mainCache, index, tag, creation_time);

   
  }

  //no-write-allocate: writes straight to memory, does not load into cache
  if (write_miss == "no-write-allocate") {
    mainCache->total_cycles += 100;
  }
  
}


/*
 * Performs operations that happen in a store hit for LRU implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned tag indicating the tag that was found in cache
 *   write_hit - a string indicaitng what behavior to perform when a 
 *    write_hit occurs
 *   slot_index - an unsigned indicating the specific slot to affect
 *
 * Returns:
 *   nothing
 */
void lruStoreHit(Cache* mainCache, unsigned index, unsigned tag, string write_hit, unsigned slot_index) {
	  
  //write-through: write immediately to memory
  if (write_hit == "write-through") {
    
    mainCache->sets[index].slots[slot_index].tag = tag;
    mainCache->total_cycles += 1 + 100;
  }
  //write-back: defer to write to memory until replacement of line
  if (write_hit == "write-back") {
    mainCache->sets[index].slots[slot_index].dirty = true;
    mainCache->total_cycles++;
  }
  
  lruFinish(mainCache, index, slot_index);
  
}


/*
 * Performs operations that happen in a store hit for FIFO implementation
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   write_hit - a string indicaitng what behavior to perform when a 
 *    write_hit occurs
 *   slot_index - an unsigned indicating the specific slot to affect
 *
 * Returns:
 *   nothing
 */
void fifoStoreHit(Cache* mainCache, unsigned index, unsigned slot_index, string write_hit) {
  //write-through: write immediately to memory
  if (write_hit == "write-through") {
    mainCache->total_cycles += 1 + 100;
  }

  //write-back: defer to write to memory until replacement of line
  if (write_hit == "write-back") {
    mainCache->sets[index].slots[slot_index].dirty = true;
    mainCache->total_cycles++;
  }
  
}

/*
 * Decides if there is a hit or a miss for load based on parameters
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   key_count - an int indicating if tag was found in cache
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned indicating the tag that was looked in the cache
 *
 * Returns:
 *   nothing
 */
void loadDecisions(Cache* mainCache, int key_count, unsigned index, unsigned tag, string eviction_type, unsigned creation_time) {
  unsigned slot_index;
  //load miss
  if (key_count <= 0) {
    mainCache->load_misses++;
    //if "lru" selected then do the proper operations
    if (eviction_type == "lru") {
      lruLoadMiss(mainCache, index, tag);
    } else  {
      //otherwise do the proper fifo operations
      fifoLoadMiss(mainCache, index, tag, creation_time);
    }
    //load---- hit
  } else {
    mainCache->load_hits++;
    slot_index = mainCache->sets[index].directory[tag];
    //if "lru" selected then do the proper operations
    if (eviction_type == "lru") {
      lruLoadHit(mainCache, index, slot_index);
    } else {
      //otherwise do the proper fifo operations
      fifoLoadHit(mainCache);
    }
    
  }
  
  mainCache->total_loads++;
}


/*
 * Decides between hit or miss for store and directs to proper functions
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   key_count - an int indicating if tag was found in cache
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned indicating the tag that was looked in the cache
 *   write_miss - a string indicating what behavior to perform during a
 *    write_miss
 *   write_hit - a string indicating what to do in case of a write_hit
 *
 *
 * Returns:
 *   nothing
 */
void storeDecisions(Cache* mainCache, int key_count, unsigned index, unsigned tag, string write_miss, string write_hit, string eviction_type, unsigned creation_time) {
  unsigned slot_index;
  //store miss
  if (key_count <= 0) {
    mainCache->store_misses++;
    //if "lru" selected then do the proper operations
    if (eviction_type == "lru") {
      storeMiss(mainCache, index, tag, write_miss, write_hit);
    } else  {
      //otherwise do the proper fifo operations
      fifoStoreMiss(mainCache, index, tag, write_miss, write_hit, creation_time);
    }
    //store-hit
  } else {
    mainCache->store_hits++;
    slot_index = mainCache->sets[index].directory[tag];
    //if "lru" selcted then do the proper operations
    if (eviction_type == "lru") {
      lruStoreHit(mainCache, index, tag, write_hit, slot_index);
    } else {
      //otherwise do the proper fifo operations
      fifoStoreHit(mainCache, index, slot_index, write_hit);
    }
  }
  mainCache->total_stores++;
}


/*
 * Checks for either load or store and directs to appropriate functions
 * 
 * Parameters:
 *   mainCache - a pointer to the cache to be affected
 *   index - an unsigned indicating the index in the cache to affect
 *   tag - an unsigned indicating the tag that was looked in the cache
 *   load_store - a char that indicates either a load or a store to the cache
 *   write_miss - a string indicating what behavior to perform during a
 *    write_miss
 *   write_hit - a string indicating what to do in case of a write_hit
 *
 *
 * Returns:
 *   nothing
 */
void loadStoreWork(Cache* mainCache, unsigned index, unsigned tag, char load_store, string write_miss, string write_hit, string eviction_type, unsigned creation_time) {
  //search the set to find the index of the slot that has the tag
  int key_count = mainCache->sets[index].directory.count(tag);
  
  //if loading do the proper operations
  if (load_store == 'l') {
    loadDecisions(mainCache, key_count, index, tag, eviction_type, creation_time);
    //otherwise do the proper storing operations
  } else {
    
    storeDecisions(mainCache, key_count, index, tag, write_miss, write_hit, eviction_type, creation_time);
  }
  
}


/*
 * Decides between hit or miss for store and directs to proper functions
 * 
 * Parameters:
 *  argc - an int indicating the number of paramenters given at command line
 *  argv - an array of characters that contains all info given at command line
 *
 * Returns:
 *   1 if error happens 0 if everything finishes as normal
 */
int main(int argc, char* argv[]) {
  
  //print an error if invalid number of parameters
  if (argc != 7) {
    cerr << "error: invalid number of parameters";
    return 1;
  }
  
  //store inputted info int their proper spots
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

  //checks to see if any invalid combinations inputted if so prints error and returns 1
  if (invalids(num_sets, block_size, num_block_bytes, write_hit, write_miss, eviction_type, set, block, block_bytes) == 1) {
    return 1;
  }

  //set up the cache
  Cache mainCache;
  cache_setup(&mainCache, num_sets, block_size, num_block_bytes);

  //variables for valuable info
  char load_store;
  string address;
  string third_var;
  string line;

  // variable that dictates when the info on a particular block was put there/created
  unsigned creation_time = 1;
  
  while (getline(cin,line)) {
    //store each memory access field
    stringstream ss;
    ss << line;
    ss >> load_store >> address >> third_var;
    
    //convert the address to binary
    string binary_form = hexToBinary(address.substr(2, 8));
    
    //get the binary form of the tag and index from the address
    int total_cache_capacity = log2(mainCache.num_sets * mainCache.container_size);
    int num_offset_digits = log2(mainCache.container_size);
    int num_index_digits = total_cache_capacity - num_offset_digits;
    int num_tag_digits = 32 - num_offset_digits - num_index_digits;
    string tag_str = binary_form.substr(0, num_tag_digits);
    string index_str = "";
    int string_size = binary_form.size();
    if (num_tag_digits + num_index_digits <= string_size) {
      index_str = binary_form.substr(num_tag_digits, num_index_digits);
    }
    //convert binary tag and index to decimal value
    unsigned tag = 0;
    if (tag_str.length() > 0) {
      tag = stoi(tag_str, 0, 2);
    }
    unsigned index = 0;
    if (index_str.length() > 0) {
      index = stoi(index_str, 0, 2);
    }

    // gives all relevant info and decides what operation should be taken based on current parameters
    loadStoreWork(&mainCache, index, tag, load_store, write_miss, write_hit, eviction_type, creation_time);
    // increase creation time variable by 1
    creation_time++;
    
  }

  //prints all the final stats in a nice and easy to read format
  final_print(&mainCache);
  return 0;
}
