// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!


int main(void) {
  char data_buf[18];
  char complete_hex_output[49];
  char offset_buf[11];
  char single_conversion[3];
  int char_read;
  int offset = 0;

  //variables for storing 16 character string
  char hex_read_buf[16];
  char buf_overflow[16];
  int chars_in_data_buf = 0;
  int z;
  int how_many_overflow = 0;

  
  do {

    for (chars_in_data_buf = 0; chars_in_data_buf < how_many_overflow; chars_in_data_buf++) {
      data_buf[chars_in_data_buf] = buf_overflow[chars_in_data_buf];
    }
    
    how_many_overflow = 0;

    do {
      char_read = hex_read(hex_read_buf);
      for (z = 0; z < char_read && (chars_in_data_buf + z) < 16; z++) {
	data_buf[chars_in_data_buf + z] = hex_read_buf[z];
      }
      chars_in_data_buf += z;
      for (z = z; z < char_read; z++) {
	buf_overflow[how_many_overflow++] = hex_read_buf[z];
      }
    } while (chars_in_data_buf < 17 && char_read > 0);



    
    //print the position in the input as hexadecimal number
    hex_format_offset(offset, offset_buf);
    offset_buf[8] = ':';
    offset_buf[9] = ' ';
    offset_buf[10] = '\0';
    hex_write_string(offset_buf);
    
    int i;
    int complete_counter = 0;
    
    //for each character, add hex representation of their ASCII value and a space to the final string
    for (i = 0; i < chars_in_data_buf; i++) {
      single_conversion[0] = data_buf[i];
      hex_format_byte_as_hex(single_conversion[0], single_conversion);
      single_conversion[2] = ' ';
      
      for (int j = 0; j < 3; j++) {
	complete_hex_output[complete_counter + j] = single_conversion[j];
      }
      complete_counter += 3;
      
    }
    //add spacing between ASCII hex representation and string-like representation as necessary
    while (complete_counter < 50) {
      complete_hex_output[complete_counter++] = ' ';
    }
    complete_hex_output[complete_counter] = '\0';
    
    //print the final string
    hex_write_string(complete_hex_output);
   
    //form and print string-like representation
    for (int j = 0; j < chars_in_data_buf; j++) {
      data_buf[j] = hex_to_printable(data_buf[j]);
    }
    data_buf[chars_in_data_buf] = '\n';
    data_buf[chars_in_data_buf + 1] = '\0';
    hex_write_string(data_buf);
    
    //move onto next 16 bytes
    offset += 16;
    //char_read = hex_read(data_buf);
    chars_in_data_buf = 0;

    
  } while (how_many_overflow > 0);
  
}
