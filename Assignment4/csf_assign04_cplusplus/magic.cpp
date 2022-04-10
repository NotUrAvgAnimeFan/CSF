#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>

#include "elf_names.h"
using namespace std;

void printHeaderStats(Elf64_Ehdr *elf_header) {
  
  const char* objectType = get_type_name(elf_header->e_type);
  const char* machineType = get_machine_name(elf_header->e_machine);
  
  cout << "Object file type: " << objectType << endl;
  cout << "Instruction set: " << machineType << endl;
  
  if ((int)elf_header->e_ident[EI_DATA] == 1) {
    cout << "Endianness: Little endian" << endl;
  } else {
    cout << "Endianness: Big endian" << endl;
  }
  
}


int main(int argc, char **argv) {
  // TODO: implement
  
  int fd = open(argv[1], O_RDONLY);

  if (fd < 0) {
    cerr << "could not open file";
    return -1;
  }

  struct stat statbuf;
  size_t file_size;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {

    //error
    cerr << "could not get info on file";
    return -2;
  } else {
    file_size = statbuf.st_size;
  }

  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (data == ((void *)-1)) {
    cerr << "could not get contents of file";
    return -3;
  }

  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;

  if ((int)elf_header->e_ident[EI_MAG1] != 69 || (int)elf_header->e_ident[EI_MAG2] != 76 || (int)elf_header->e_ident[EI_MAG3] != 70) {
    printf("Not an ELF file\n");
    return 0;
  }

  printHeaderStats(elf_header);

  Elf64_Shdr *section_header = (Elf64_Shdr*)((char*)elf_header + elf_header->e_shoff);

  
  Elf64_Shdr *section_with_names = &section_header[elf_header->e_shstrndx];
  //data + section_wit_names->sh_offset + sec_header->sh_name;
  uint8_t counter = 0;
  Elf64_Shdr *start_names = (Elf64_Shdr*)(section_with_names->sh_offset + (char*)elf_header);  
  //char nameOfSection[4] = {'y', 'e', 's', '\0'};
  
  while (counter < elf_header->e_shnum) {
    
    printf("Section header %d: name=%s, type=%lx, offset=%lx, size=%lx\n", counter, (char*)start_names + section_header[counter].sh_name , section_header[counter].sh_type, section_header[counter].sh_offset, section_header[counter].sh_size);
    counter += 1;

  }

  
  counter = 0;
  uint8_t symbol_counter_top;
  uint8_t symbol_counter_current;
  while (counter < elf_header->e_shnum) {
    symbol_counter_top = section_header[counter].sh_size / section_header[counter].sh_entsize;
    
    
    
  }
  
  
}
