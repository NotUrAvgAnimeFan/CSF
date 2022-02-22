// C implementation of hexdump functions

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

  i++;
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
  
  /*
  char comparison = '\0';
  int counter = 0;
  while (counter < 255 && comparison != byteval) {
    comparison++;
    counter++;
  }
  format_as_hex_variable_string_size(2, counter, sbuf);
  */
  
  format_as_hex_variable_string_size(2, byteval, sbuf);

}

char hex_to_printable(unsigned char byteval) {
  //'.' for non-printable characters
  if (byteval < 32 || byteval > 127) {
    return 46;
  }

  return byteval;
  
}
