// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  // TODO: implement the main function
  char data_buf[16];
  char offset_buf[10];
  char converted_buf[2];
  int char_read;
  int offset = 16;
  do {
    hex_format_offset(offset, offset_buf);
    offset_buf[8] = ':';
    offset_buf[9] = ' ';
    hex_write_string(offset_buf);
    
    char_read = hex_read(data_buf);

    for (int i = 0; i < 16; i += 2) {
      
    }

    
    
    hex_write_string(data_buf);
    offset += 16;
  } while (char_read > 0);
  
  
  
}
