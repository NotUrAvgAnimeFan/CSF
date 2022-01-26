/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"

ApInt *apint_create_from_u64(uint64_t val) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_create_from_hex(const char *hex) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

void apint_destroy(ApInt *ap) {
	/* TODO: implement */
	assert(0);
}

int apint_is_zero(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return 0;
}

int apint_is_negative(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return 0;
}

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
	/* TODO: implement */
	assert(0);
	return 0UL;
}

int apint_highest_bit_set(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return -1;
}

char *apint_format_as_hex(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_negate(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	assert(0);
	return NULL;
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

int apint_compare(const ApInt *left, const ApInt *right) {
	/* TODO: implement */
	assert(0);
	return 0;
}

ApInt *apint_lshift(ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
	/* TODO: implement */
	assert(0);
	return NULL;
}