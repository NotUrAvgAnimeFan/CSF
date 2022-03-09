
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

  //print an error if invalid number of parameters
  if (argc != 7) {
    std::cerr << "error: invalid number of parameters";
    return 1;
  }
  int num_sets = atoi(argv[1]);
  int block_size = atoi(argv[2]);
  int num_block_bytes = atoi(argv[3]);
  string write_miss = argv[4];
  string write_hit = argv[5];
  string eviction_type = argv[6];


  //if cache parameters are invalid, print and error message and exit
  if ((num_sets & (num_sets - 1)) != 0 || num_sets == 0) {
    std::cerr << "error: number of sets isn't a power of 2" << endl;
    return 1;
  }
  if (block_size == 0 || (block_size & (block_size - 1)) != 0) {
    std::cerr << "error: block size isn't a power of 2" << endl;
    return 1;
  }
  if (block_size < 4) {
    std::cerr << "error: block size is less than 4" <<endl;
    return 1;
  }
  if (write_hit.compare("write-back") == 0 && write_miss.compare("no-write-allocate") == 0) {
    std::cerr << "error: write-back and no-write-allocate both specified" << endl;
    return 1;
  }

  //TODO: check that the integers are valid (ie not negative or doubles) and the strings are valid 
  //atoi returns 0 if string, removes decimals for doubles, and keeps negative values negative
  return 0;
}
