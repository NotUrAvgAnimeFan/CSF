// Function prototypes for hexdump functions
// Do not change these!
// Ricardo Manuel Morales Gonzalez  rmorale5@jhu.edu
// Ana Kuri  auri1@jhu.edu


#ifndef HEXFUNCS_H
#define HEXFUNCS_H

// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[]);

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[]);

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(unsigned offset, char sbuf[]);

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]);

// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval);


// Calculates the size of the given string and returns it
int string_size(const char s[]);


// Converts offset to hex and stores it in the desired space in sbuf
void format_as_hex_variable_string_size(int size, unsigned offset, char sbuf[]);


// prints out the correct offset with formatting
void correct_offset_formatting(int offset, char offset_buf[]);

// calculates and prints out the full hex representation of current 16 char string
void full_hex_representation_printed(int complete_counter, int chars_in_data_buf, char data_buf[], char single_conversion[], char complete_hex_output[]);

// prints out the literal string being currently converted into hex representation
void complete_string_printed(int chars_in_data_buf, char data_buf[]);

#endif // HEXFUNCS_H
