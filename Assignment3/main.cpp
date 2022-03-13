
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;

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
  
  while (getline(cin,line)) {
    //store each memory access field
    stringstream ss;
    ss << line;
    ss >> load_store >> address >> third_var;
  }
  return 0;
}
