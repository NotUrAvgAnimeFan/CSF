// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

// Ricardo Manuel Morales Gonzalez  rmorale5@jhu.edu
// Ana Kuri  auri1@jhu.edu


#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  char test_data_2[16];
  char test_data_3[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  strcpy(objs->test_data_2, "This sample\n");
  strcpy(objs->test_data_3, "CSF program\0");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);

void testMoreFormatOffset(TestObjs *objs);
void testMoreFormatByteAsHex(TestObjs *objs);
void testMoreHexToPrintable(TestObjs *objs);





int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);
  
  TEST(testMoreFormatOffset);
  TEST(testMoreFormatByteAsHex);
  TEST(testMoreHexToPrintable);

  
  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  
  ASSERT(0 == strcmp(buf, "48"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
}

void testMoreFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(2L, buf);
  ASSERT(0 == strcmp(buf, "00000002"));

  hex_format_offset(15L, buf);
  ASSERT(0 == strcmp(buf, "0000000f"));

  hex_format_offset(255L, buf);
  ASSERT(0 == strcmp(buf, "000000ff"));

  hex_format_offset(0L, buf);
  ASSERT(0 == strcmp(buf, "00000000"));

  hex_format_offset(4294967295, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
  
}

void testMoreFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));

  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));

  hex_format_byte_as_hex(objs->test_data_2[0], buf);
  ASSERT(0 == strcmp(buf, "54"));

  hex_format_byte_as_hex(objs->test_data_2[4], buf);
  ASSERT(0 == strcmp(buf, "20"));

  hex_format_byte_as_hex(objs->test_data_3[11], buf);
  ASSERT(0 == strcmp(buf, "00"));

  hex_format_byte_as_hex(objs->test_data_3[2], buf);
  ASSERT(0 == strcmp(buf, "46"));
}

void testMoreHexToPrintable(TestObjs *objs) {
  
  ASSERT(' ' == hex_to_printable(objs->test_data_1[6]));
  ASSERT('!' == hex_to_printable(objs->test_data_1[12]));

  ASSERT('.' == hex_to_printable(objs->test_data_2[11]));
  ASSERT('T' == hex_to_printable(objs->test_data_2[0]));
  ASSERT('s' == hex_to_printable(objs->test_data_2[3]));

  ASSERT('C' == hex_to_printable(objs->test_data_3[0]));
  ASSERT('S' == hex_to_printable(objs->test_data_3[1]));
  ASSERT('F' == hex_to_printable(objs->test_data_3[2]));
  ASSERT('.' == hex_to_printable(objs->test_data_3[11]));

}
