
// C implementation of hexdump functions

// Ricardo Manuel Morales Gonzalez  rmorale5@jhu.edu
// Ana Kuri  auri1@jhu.edu



#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

unsigned hex_read(char data_buf[]) {
  unsigned bytes_read = read(0, data_buf, 16);

  return bytes_read;
}

void hex_write_string(const char s[]) {
  
  write(1, s, string_size(s));
}

int string_size(const char s[]) {
  int i = 0;

  while (s[i] != '\0') {
    i++;
  }

  return i;
}


void hex_format_offset(unsigned offset, char sbuf[]) {
  format_as_hex_variable_string_size(8, offset, sbuf);
}

void format_as_hex_variable_string_size(int size, unsigned offset, char sbuf[]) {
  int charCounter = size - 1;
  sbuf[size] = '\0';
  unsigned offsetCopy;
  int difference;
  while (charCounter >= 0) {
    //isolate 4 bits
    offsetCopy = offset;
    offset = offset >> 4;
    offset = offset << 4;
    difference = offsetCopy - offset;
    
    //figure out the hex digit and add it to the string
    if (difference < 10) {
      sbuf[charCounter] = '0' + difference;
    } else {
      sbuf[charCounter] = 'a' + (difference - 10);
    }
    offset = offset >> 4;
    charCounter--;
  }
  
}

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
  
  format_as_hex_variable_string_size(2, byteval, sbuf);

}

char hex_to_printable(unsigned char byteval) {
  //'.' for non-printable characters
  if (byteval < 32 || byteval > 127) {
    return 46;
  }

  return byteval;
  
}


void correct_offset_formatting(int offset, char offset_buf[]) {
  hex_format_offset(offset, offset_buf);
  offset_buf[8] = ':';
  offset_buf[9] = ' ';
  offset_buf[10] = '\0';
  hex_write_string(offset_buf);
}


void full_hex_representation_printed(int complete_counter, int chars_in_data_buf, char data_buf[], char single_conversion[], char complete_hex_output[]) {
  //for each character, add hex representation of their ASCII value and a space to the final string
  for (int i = 0; i < chars_in_data_buf; i++) {
    single_conversion[0] = data_buf[i];
    hex_format_byte_as_hex(single_conversion[0], single_conversion);
    single_conversion[2] = ' ';

    for (int j = 0; j < 3; j++) {
      complete_hex_output[complete_counter + j] = single_conversion[j];
    }
    complete_counter +=3;
  }

  //add spacing between ASCII hex representation and string-like representation as necessary
  while (complete_counter < 49) {
    complete_hex_output[complete_counter++] = ' ';
  }
  complete_hex_output[complete_counter] = '\0';
  
  //print the final string
  hex_write_string(complete_hex_output);
}


void complete_string_printed(int chars_in_data_buf, char data_buf[]) {
  for (int j = 0; j < chars_in_data_buf; j++) {
    data_buf[j] = hex_to_printable(data_buf[j]);
  }
  data_buf[chars_in_data_buf] = '\n';
  data_buf[chars_in_data_buf + 1] = '\0';
  hex_write_string(data_buf);
}
