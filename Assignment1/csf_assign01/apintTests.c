/*
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"

typedef struct {
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
	ApInt *minus1;
	/* TODO: add additional fields of test fixture */
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
/* TODO: add more test function prototypes */

int main(int argc, char **argv) {
	TEST_INIT();

	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}

	TEST(testCreateFromU64);
	TEST(testHighestBitSet);
	TEST(testCompare);
	TEST(testFormatAsHex);
	TEST(testAdd);
	TEST(testSub);
	/* TODO: use TEST macro to execute more test functions */

	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_u64(0UL);
	objs->ap1 = apint_create_from_u64(1UL);
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
	objs->minus1 = apint_negate(objs->ap1);
	/* TODO: initialize additional members of test fixture */

	return objs;
}

void cleanup(TestObjs *objs) {
	apint_destroy(objs->ap0);
	apint_destroy(objs->ap1);
	apint_destroy(objs->ap110660361);
	apint_destroy(objs->max1);
	apint_destroy(objs->minus1);
	/* TODO: destroy additional members of test fixture */

	free(objs);
}

void testCreateFromU64(TestObjs *objs) {
	ASSERT(0UL == apint_get_bits(objs->ap0, 0));
	ASSERT(1UL == apint_get_bits(objs->ap1, 0));
	ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
}

void testHighestBitSet(TestObjs *objs) {
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
}

void testCompare(TestObjs *objs) {
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);
}

void testFormatAsHex(TestObjs *objs) {
	char *s;

	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);
}

void testAdd(TestObjs *objs) {
	ApInt *sum;
	char *s;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 0 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 110660361 + 1 = 110660362 */
	sum = apint_add(objs->ap110660361, objs->ap1);
	ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);
}

void testSub(TestObjs *objs) {
	ApInt *a, *b, *diff;
	char *s;

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 0 is -1 */
	diff = apint_sub(objs->ap0, objs->ap1);
	ASSERT(0 == (strcmp("-1", (s = apint_format_as_hex(diff)))));
	ASSERT(0 == apint_compare(diff, objs->minus1));
	apint_destroy(diff);
	free(s);

	/* test involving larger values */
	a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);

	/* test involving larger values (with a negative difference) */
	a = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	b = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("-7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);
}

/* TODO: add more test functions */
