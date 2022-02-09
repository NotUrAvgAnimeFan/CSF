#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint max;

  Fixedpoint min;
  Fixedpoint neg_min;

} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);

void test_simple_addition(TestObjs *objs);
void test_simple_subtraction(TestObjs *objs);
void test_simple_doubling(TestObjs *objs);
void test_simple_halving(TestObjs *objs);
void test_create_more_from_hex(TestObjs *objs);
void test_more_negate(TestObjs *objs);
void test_compare(TestObjs *objs);
void test_is_overflow_neg(TestObjs *objs);
void test_is_underflow_pos(TestObjs *objs);
void test_is_underflow_neg(TestObjs *objs);

int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_is_overflow_pos);
  TEST(test_is_err);

  
  TEST(test_simple_addition);
  TEST(test_simple_subtraction);
  TEST(test_simple_doubling);
  TEST(test_simple_halving);
  TEST(test_create_more_from_hex);
  TEST(test_more_negate);
  TEST(test_compare);
  TEST(test_is_overflow_neg);
  TEST(test_is_underflow_pos);
  TEST(test_is_underflow_neg);
    

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->max = fixedpoint_create2(0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL);
  objs->min = fixedpoint_create_from_hex("0.0000000000000001");
  objs->neg_min = fixedpoint_create_from_hex("-0.0000000000000001");
  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");

  ASSERT(fixedpoint_is_valid(val1));

  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));

  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
}

void test_negate(TestObjs *objs) {
    
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

void test_simple_addition(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("1");
  rhs = fixedpoint_create_from_hex("1");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(sum)));
  ASSERT(0x2UL == fixedpoint_whole_part(sum));

  lhs = fixedpoint_create_from_hex("-1");
  rhs = fixedpoint_create_from_hex("-1");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x2UL == fixedpoint_whole_part(sum));

  lhs = fixedpoint_create_from_hex("1");
  rhs = fixedpoint_create_from_hex("-2");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x1UL == fixedpoint_whole_part(sum));

  lhs = fixedpoint_create_from_hex("-2");
  rhs = fixedpoint_create_from_hex("1");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x1UL == fixedpoint_whole_part(sum));
  
  lhs = fixedpoint_create_from_hex("0.7");
  rhs = fixedpoint_create_from_hex("0.8");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(sum)));
  ASSERT(0xf000000000000000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("0.8");
  rhs = fixedpoint_create_from_hex("0.8");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(sum)));
  ASSERT(0x1UL == fixedpoint_whole_part(sum));
  ASSERT(0x0UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("0.7");
  rhs = fixedpoint_create_from_hex("-0.8");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(sum));

  lhs = fixedpoint_create_from_hex("-0.8");
  rhs = fixedpoint_create_from_hex("0.7");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(sum));
  
}

void test_simple_subtraction(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("2");
  rhs = fixedpoint_create_from_hex("1");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0x1UL == fixedpoint_whole_part(diff));

  lhs = fixedpoint_create_from_hex("1");
  rhs = fixedpoint_create_from_hex("2");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0x1UL == fixedpoint_whole_part(diff));

  lhs = fixedpoint_create_from_hex("-2");
  rhs = fixedpoint_create_from_hex("-1");
  ASSERT(fixedpoint_is_neg(lhs));
  ASSERT(fixedpoint_is_neg(rhs));
  
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0x1UL == fixedpoint_whole_part(diff));

  lhs = fixedpoint_create_from_hex("-1");
  rhs = fixedpoint_create_from_hex("-2");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0x1UL == fixedpoint_whole_part(diff));

  lhs = fixedpoint_create_from_hex("2");
  rhs = fixedpoint_create_from_hex("-1");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0x3UL == fixedpoint_whole_part(diff));

  lhs = fixedpoint_create_from_hex("-1");
  rhs = fixedpoint_create_from_hex("2");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0x3UL == fixedpoint_whole_part(diff));

  lhs = fixedpoint_create_from_hex("1");
  rhs = fixedpoint_create_from_hex("1");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0x0UL == fixedpoint_whole_part(diff));
  ASSERT(fixedpoint_is_zero(diff));

  lhs = fixedpoint_create_from_hex("-1");
  rhs = fixedpoint_create_from_hex("-1");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0X0UL == fixedpoint_whole_part(diff));
  ASSERT(fixedpoint_is_zero(diff));

  lhs = fixedpoint_create_from_hex("0.7");
  rhs = fixedpoint_create_from_hex("0.8");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(diff));

  lhs = fixedpoint_create_from_hex("0.8");
  rhs = fixedpoint_create_from_hex("0.7");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(diff));
  
  lhs = fixedpoint_create_from_hex("0.7");
  rhs = fixedpoint_create_from_hex("-0.8");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0xf000000000000000UL == fixedpoint_frac_part(diff));

  lhs = fixedpoint_create_from_hex("-0.7");
  rhs = fixedpoint_create_from_hex("0.8");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0Xf000000000000000UL == fixedpoint_frac_part(diff));
  
  lhs = fixedpoint_create_from_hex("-0.7");
  rhs = fixedpoint_create_from_hex("-0.8");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(diff));

  lhs = fixedpoint_create_from_hex("-0.8");
  rhs = fixedpoint_create_from_hex("-0.7");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(diff));

  lhs = fixedpoint_create_from_hex("-0.f");
  rhs = fixedpoint_create_from_hex("0.2");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0x1UL == fixedpoint_whole_part(diff));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(diff));

  lhs = fixedpoint_create_from_hex("0.f");
  rhs = fixedpoint_create_from_hex("-0.2");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(!(fixedpoint_is_neg(diff)));
  ASSERT(0x1UL == fixedpoint_whole_part(diff));
  ASSERT(0x1000000000000000UL == fixedpoint_frac_part(diff));
  
}

void test_simple_doubling(TestObjs *objs) {
  (void) objs;

  Fixedpoint toDouble, result;

  toDouble = fixedpoint_create_from_hex("1");
  result = fixedpoint_double(toDouble);
  ASSERT(!(fixedpoint_is_neg(result)));
  ASSERT(0x2UL == fixedpoint_whole_part(result));

  toDouble = fixedpoint_create_from_hex("-1");
  result = fixedpoint_double(toDouble);
  ASSERT(fixedpoint_is_neg(result));
  ASSERT(0x2UL == fixedpoint_whole_part(result));

  toDouble = fixedpoint_create_from_hex("d");
  result = fixedpoint_double(toDouble);
  ASSERT(!(fixedpoint_is_neg(result)));
  ASSERT(0x1aUL == fixedpoint_whole_part(result));


  toDouble = fixedpoint_create_from_hex("0.4");
  result = fixedpoint_double(toDouble);
  ASSERT(!(fixedpoint_is_neg(result)));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(result));

  toDouble = fixedpoint_create_from_hex("-0.4");
  result = fixedpoint_double(toDouble);
  ASSERT(fixedpoint_is_neg(result));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(result));

  toDouble = fixedpoint_create_from_hex("0.9");
  result = fixedpoint_double(toDouble);
  ASSERT(!(fixedpoint_is_neg(result)));
  ASSERT(0x1UL == fixedpoint_whole_part(result));
  ASSERT(0x2000000000000000UL == fixedpoint_frac_part(result));
  
  toDouble = fixedpoint_create_from_hex("-0.9");
  result = fixedpoint_double(toDouble);
  ASSERT(fixedpoint_is_neg(result));
  ASSERT(0x1UL == fixedpoint_whole_part(result));
  ASSERT(0x2000000000000000UL == fixedpoint_frac_part(result));

  
  toDouble = fixedpoint_create_from_hex("8000000000000000");
  result = fixedpoint_double(toDouble);
  ASSERT(!(fixedpoint_is_neg(result)));
  ASSERT(0x0UL == fixedpoint_whole_part(result));
  ASSERT(fixedpoint_is_overflow_pos(result));

  toDouble = fixedpoint_create_from_hex("-8000000000000000");
  result = fixedpoint_double(toDouble);
  ASSERT(fixedpoint_is_overflow_neg(result));
  
}

void test_simple_halving(TestObjs *objs) {
  (void) objs;

  Fixedpoint toHalve, result;
  toHalve = fixedpoint_create_from_hex("4");
  result = fixedpoint_halve(toHalve);
  ASSERT(!(fixedpoint_is_neg(result)));
  ASSERT(0x2UL == fixedpoint_whole_part(result));

  toHalve = fixedpoint_create_from_hex("-4");
  result = fixedpoint_halve(toHalve);
  ASSERT(fixedpoint_is_neg(result));
  ASSERT(0x2UL == fixedpoint_whole_part(result));

  toHalve = fixedpoint_create_from_hex("1");
  result = fixedpoint_halve(toHalve);
  ASSERT(!(fixedpoint_is_neg(result)));
  ASSERT(0x0UL == fixedpoint_whole_part(result));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(result));

  toHalve = fixedpoint_create_from_hex("-1");
  result = fixedpoint_halve(toHalve);
  ASSERT(fixedpoint_is_neg(result));
  ASSERT(0x0UL == fixedpoint_whole_part(result));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(result));
  
  
}

void test_create_more_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint result;

  result = fixedpoint_create_from_hex(" ");
  ASSERT(fixedpoint_is_err(result));

  result = fixedpoint_create_from_hex(".");
  ASSERT(fixedpoint_is_err(result));

  result = fixedpoint_create_from_hex("..");
  ASSERT(fixedpoint_is_err(result));
  
  result = fixedpoint_create_from_hex("-");
  ASSERT(fixedpoint_is_err(result));

  result = fixedpoint_create_from_hex("--");
  ASSERT(fixedpoint_is_err(result));

  result = fixedpoint_create_from_hex("-0.0");
  ASSERT(result.valid_nonnegative == true);
  ASSERT(!(fixedpoint_is_err(result)));

  result = fixedpoint_create_from_hex("");
  ASSERT(fixedpoint_is_err(result));

  result = fixedpoint_create_from_hex("aaaaaaaaaaaaaaaa");
  ASSERT(!(fixedpoint_is_err(result)));

  result = fixedpoint_create_from_hex("aaaaaaaaaaaaaaaaa");
  ASSERT(fixedpoint_is_err(result));

  
  
}

void test_more_negate(TestObjs *objs) {
  (void) objs;
  
  Fixedpoint toNegate, result;
  toNegate = fixedpoint_create_from_hex("-1");
  ASSERT(fixedpoint_is_neg(toNegate));
  ASSERT(0x1UL == fixedpoint_whole_part(toNegate));
  result = fixedpoint_negate(toNegate);
  ASSERT(0x1UL == fixedpoint_whole_part(result));
  ASSERT(!(fixedpoint_is_neg(result)));

  toNegate = fixedpoint_create_from_hex("1");
  result = fixedpoint_negate(toNegate);
  ASSERT(fixedpoint_is_neg(result));
  ASSERT(0x1UL == fixedpoint_whole_part(result));
  
}

void test_compare(TestObjs *objs) {
  (void) objs;
  Fixedpoint f1, f2;
  f1 = fixedpoint_create_from_hex("1");
  f2 = fixedpoint_create_from_hex("1");
  ASSERT(0 == fixedpoint_compare(f1, f2));
  
  f1 = fixedpoint_create_from_hex("-1");
  ASSERT(-1 == fixedpoint_compare(f1, f2));
  
  f1 = fixedpoint_create_from_hex("5");
  ASSERT(1 == fixedpoint_compare(f1, f2));
  
  f1 = fixedpoint_create_from_hex("1.5");
  f2 = fixedpoint_create_from_hex("1.5");
  ASSERT(0 == fixedpoint_compare(f1, f2));
  
  f1 = fixedpoint_create_from_hex("1.2");
  ASSERT(-1 == fixedpoint_compare(f1, f2));
  
  f1 = fixedpoint_create_from_hex("1.7");
  ASSERT(1 == fixedpoint_compare(f1, f2));
  
  f1 = fixedpoint_create_from_hex("2.3");
  ASSERT(1 == fixedpoint_compare(f1, f2));
  
  f1 = fixedpoint_create_from_hex("0.5");
  f2 = fixedpoint_create_from_hex("0.6");
  ASSERT(-1 == fixedpoint_compare(f1, f2));
}

void test_is_overflow_neg(TestObjs *objs) {
  
  Fixedpoint value1, value2, diff;
  
  value1 = fixedpoint_create_from_hex("-ffffffffffffffff");
  
  diff = fixedpoint_sub(value1, objs->one);
  ASSERT(fixedpoint_is_overflow_neg(diff));

  
  value1 = fixedpoint_create_from_hex("ffffffffffffffff");
  value2 = fixedpoint_create_from_hex("-1");
  diff = fixedpoint_sub(value1, value2);
  ASSERT(fixedpoint_is_overflow_pos(diff));
  ASSERT(fixedpoint_is_zero(diff));

  value1 = fixedpoint_create_from_hex("80000000000000000");
  value2 = fixedpoint_create_from_hex("90000000000000000");
  
}

void test_is_underflow_pos(TestObjs *objs) {
  Fixedpoint value1, result;


  result = fixedpoint_halve(objs->min);
  ASSERT(fixedpoint_is_underflow_pos(result));

  value1 = fixedpoint_create_from_hex("0.123400000000000f");
  result = fixedpoint_halve(value1);
  ASSERT(fixedpoint_is_underflow_pos(result)); 

  
}

void test_is_underflow_neg(TestObjs *objs){   
  Fixedpoint value1, result;

  result = fixedpoint_halve(objs->neg_min);
  ASSERT(fixedpoint_is_underflow_neg(result));

  value1 = fixedpoint_create_from_hex("-0.123400000000000f");
  ASSERT(fixedpoint_is_neg(value1));

  result = fixedpoint_halve(value1);
  ASSERT(fixedpoint_is_underflow_neg(result));
  
  
}
