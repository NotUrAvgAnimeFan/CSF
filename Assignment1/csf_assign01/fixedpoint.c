#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
  // TODO: implement
  Fixedpoint made;
  made.integer1 = whole;
  made.integer2 = 0;
  made.valid_nonnegative = true;
  made.valid_negative = false;
  made.error = false;
  made.positive_overflow = false;
  made.negative_overflow = false;
  made.positive_underflow = false;
  made.negative_underflow = false;
  
  return made;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  
  Fixedpoint made;
  made.integer1 = whole;
  made.integer2 = frac;
  made.valid_nonnegative = true;
  made.valid_negative = false;
  made.error = false;
  made.positive_overflow = false;
  made.negative_overflow = false;
  made.positive_underflow = false;
  made.negative_underflow = false;
  return made;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.integer1;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.integer2;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_zero(Fixedpoint val) {
  // TODO: implement
  if (val.valid_nonnegative == false && val.valid_negative == false && (val.integer1 != 0 || val.integer2 != 0)) {
    return 0;
  }
  return 1;
}

int fixedpoint_is_err(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
