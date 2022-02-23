// C implementation of hexdump main function
// Ricardo Manuel Morales Gonzalez  rmorale5@jhu.edu
// Ana Kuri  auri1@jhu.edu


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


    //puts queud chars into data_buf 
    for (chars_in_data_buf = 0; chars_in_data_buf < how_many_overflow; chars_in_data_buf++) {
      data_buf[chars_in_data_buf] = buf_overflow[chars_in_data_buf];
    }
    
    how_many_overflow = 0;

    //continually gets chars from standard input
    do {
      char_read = hex_read(hex_read_buf);
      for (z = 0; z < char_read && (chars_in_data_buf + z) < 16; z++) {
	data_buf[chars_in_data_buf + z] = hex_read_buf[z];
      }
      chars_in_data_buf += z;
      for (z = z; z < char_read; z++) {
	buf_overflow[how_many_overflow++] = hex_read_buf[z];
      }
    } while (chars_in_data_buf < 16 && char_read > 0);


    
    
    //print the position in the input as hexadecimal number
    if (chars_in_data_buf > 0) {
     
      correct_offset_formatting(offset, offset_buf);

      
      //int i;
      int complete_counter = 0;

      //full hex representation of string will be calculated and printed
      full_hex_representation_printed(complete_counter, chars_in_data_buf, data_buf, single_conversion, complete_hex_output);
      
      
      //form and print string-like representation
      complete_string_printed(chars_in_data_buf, data_buf);
      
      
      //move onto next 16 bytes
      offset += 16;
      
      //char_read = hex_read(data_buf);
      chars_in_data_buf = 0;
    }

    
  } while (char_read > 0);
  
}
