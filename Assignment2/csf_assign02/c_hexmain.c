// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  char data_buf[18];
  char complete_hex_output[49];
  char offset_buf[11];
  char single_conversion[3];
  int char_read;
  int offset = 0;
  

  char_read = hex_read(data_buf);

  while (char_read > 0) {
    
    hex_format_offset(offset, offset_buf);
    offset_buf[8] = ':';
    offset_buf[9] = ' ';
    offset_buf[10] = '\0';
    hex_write_string(offset_buf);
    
    int i;
    int complete_counter = 0;
  
    for (i = 0; i < char_read; i++) {
      single_conversion[0] = data_buf[i];
      hex_format_byte_as_hex(single_conversion[0], single_conversion);
      single_conversion[2] = ' ';
      
      for (int j = 0; j < 3; j++) {
	complete_hex_output[complete_counter + j] = single_conversion[j];
      }
      complete_counter += 3;
      
    }

    while (complete_counter < 49) {
      complete_hex_output[complete_counter++] = ' ';
    }
    
    complete_hex_output[complete_counter] = '\0';
    
    hex_write_string(complete_hex_output);

    for (int j = 0; j < char_read; j++) {
      data_buf[j] = hex_to_printable(data_buf[j]);
    }
    data_buf[char_read] = '\n';
    data_buf[char_read + 1] = '\0';


    
    hex_write_string(data_buf);
    offset += 16;
    
    char_read = hex_read(data_buf);
  }
  
}
