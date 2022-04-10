/*
 * Work performed to get all stats from an ELF file
 * CSF Assignment 4
 * Ricardo Morales Gonzalez
 * rmorale5@jhu.edu
 * Ana Kuri
 * akuri1@jhu.edu
 */


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



/*
 * Prints the stats for the Header in the ELF file by getting the start
 * of the file as a header pointer, getting all relevant info and printing
 * it to cout
 * 
 * Parameters:
 *  elf_header - an Elf64_Ehdr pointer that points to the beginning of the elf file
 *
 * Returns:
 *  nothings since it prints all info
 *
 */

void printHeaderStats(Elf64_Ehdr *elf_header) {

  //stores objecType and machineType info  to be printed
  const char* objectType = get_type_name(elf_header->e_type);
  const char* machineType = get_machine_name(elf_header->e_machine);

  //prints relevant information
  cout << "Object file type: " << objectType << endl;
  cout << "Instruction set: " << machineType << endl;

  //Checks to see what type of Endianness there is
  if ((int)elf_header->e_ident[EI_DATA] == 1) {
    //pritn little if == 1
    cout << "Endianness: Little endian" << endl;
  } else {
    // otherwise print Big endian
    cout << "Endianness: Big endian" << endl;
  }
  
}

/*
 * Prints the stats for the Symbols in the ELF file by getting the section_hear,
 * symTab, and strTab pointers as well as the section_with_symTab number. This
 * allows the function to compute all needed information in the form of pointer
 * arithmetic
 * 
 * Parameters:
 *  section_header - an Elf64_Shdr pointer that points to the beginning of the section header
 *  section_with_symTab - a uint8_t integer that specifies at which index the section with the tab is
 *  symTab - an Elf64_Sym pointer that points to the location where the symbols tab is located in the elf file
 *  strTab - an Elf64_Sym pointer that points to the location where the names for the symbols is located  
 *
 * Returns:
 *  nothings since it prints all info
 *
 */
void printSymbolStats(Elf64_Shdr* section_header, uint8_t section_with_symTab, Elf64_Sym* symTab, Elf64_Sym* strTab) {
  // calculates the number of symbols in the symTable
  uint8_t numSym = section_header[section_with_symTab].sh_size / section_header[section_with_symTab].sh_entsize;

  // a counter to iterate through all symbols in symTable 
  uint8_t counter = 0;

  //iterates through all symbols in symTab and prints relevant information
  while (counter < numSym) {
    printf("Symbol %d: name=%s, size=%lx, info=%lx, other=%lx\n", counter, (char *)strTab + symTab[counter].st_name, symTab[counter].st_size, (long unsigned int)symTab[counter].st_info, (long unsigned int)symTab[counter].st_other);
    counter++;
  }
}


/*
 * Prints the stats for the Section Header in the ELF file by getting the start
 * of the file as a header pointer, getting all relevant info and printing
 * it to cout
 * 
 * Parameters:
 *  elf_header - an Elf64_Ehdr pointer that points to the beginning of the elf file
 *
 * Returns:
 *  nothings since it prints all info
 *
 */

void printSectionStats(Elf64_Ehdr *elf_header) {

  //computes and stores the location of the section header
  Elf64_Shdr *section_header = (Elf64_Shdr*)((char*)elf_header + elf_header->e_shoff);
  
  //computes and store the location of the section that contains name info
  Elf64_Shdr *section_with_names = &section_header[elf_header->e_shstrndx];
  //counter to keep track of items printed
  uint8_t counter = 0;
  //computes and stores the location where names start for sections
  Elf64_Shdr *start_names = (Elf64_Shdr*)(section_with_names->sh_offset + (char*)elf_header);  

  //stores where the table of symbols is located
  Elf64_Sym *symTab;
  //stores where the table of strings for symbols is located
  Elf64_Sym *strTab;
  //stores the index of the section that contains symbol table information
  uint8_t section_with_symTab;

  //iterates through all sections printing relevant information while looking
  //for info on symTab and strTab locations
  while (counter < elf_header->e_shnum) {
    //prints needed information
    printf("Section header %d: name=%s, type=%lx, offset=%lx, size=%lx\n", counter, (char*)start_names + section_header[counter].sh_name , (long unsigned int)section_header[counter].sh_type, section_header[counter].sh_offset, section_header[counter].sh_size);

    //checks to see if current section contains info on symTab
    if (section_header[counter].sh_type == 2) {
      // if so compute and store that information
      symTab = (Elf64_Sym*)((char*)elf_header + section_header[counter].sh_offset);
      section_with_symTab = counter;
    }

    //checks to see if current section contains info on strTab
    if (strcmp(".strtab",(char*)start_names + section_header[counter].sh_name) == 0) {
      // if so compute and store that information
      strTab = (Elf64_Sym*)(section_header[counter].sh_offset + (unsigned char*)elf_header);
    }

    //increase counter
    counter += 1;
    
  }

  //calls printSymbolStats to print symbol statistics
  printSymbolStats(section_header, section_with_symTab, symTab, strTab);

}

/*
 * main function that makes sure a file is able to be read and then calls the
 * necessary helper functions to print all needed information from elf file
 * 
 * Parameters:
 *  argc - an int indicating how many items were inputted as command line arguments
 *  argv - a pointer that points to various strings that were inputted as command line arguments
 *
 * Returns:
 *  0 if everything passes and a non 0 number if a problem is encountered
 *
 */
int main(int argc, char **argv) {

  //filel is opened
  int fd = -1;
  if (argc > 0) { 
    fd = open(argv[1], O_RDONLY);
  }

  //makes sure file was opened successfully
  if (fd < 0) {
    //if not print to error and return -1
    cerr << "could not open file\n";
    return -1;
  }

  //checks to see how many bytes file has
  struct stat statbuf;
  size_t file_size;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {

    //error
    cerr << "could not get info on file\n";
    return -2;
  } else {
    //appropriate size is now in file_size
    file_size = statbuf.st_size;
  }

  //creates a prive read-only mapping to start reading the file
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (data == ((void *)-1)) {
    //file could not be read print error message and return -3
    cerr << "could not get contents of file\n";
    return -3;
  }

  //turns data pointer to an elf_header pointer to start getting relevant info from file
  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;

  //checks to make sure file read is an ELF file
  if ((int)elf_header->e_ident[EI_MAG0] != 127 || (int)elf_header->e_ident[EI_MAG1] != 69 || (int)elf_header->e_ident[EI_MAG2] != 76 || (int)elf_header->e_ident[EI_MAG3] != 70) {
    //if not print message and return 0;
    printf("Not an ELF file\n");
    return 0;
  }

  //print header stats
  printHeaderStats(elf_header);

  //print section stats
  printSectionStats(elf_header);
  
}
