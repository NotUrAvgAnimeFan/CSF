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
  
  Fixedpoint made;
  int size = strlen(hex);
  int parts_size = 0;
  int z = 0;
  if (hex[z] == '-') {
    z = 1;
  }
  for (z; z < size; z++) {
    if (((hex[z] < '0' || hex [z] > '9') && (hex[z] < 'a' || hex[z] > 'f') && (hex[z] < 'A' || hex[z] > 'F') && (hex[z] != '.')) || parts_size > 16) {
      made.error = true;
      made.integer1 = 1;
      made.interger2 = 0;
      made.valid_nonnegative = false;
      made.valid_negative = false;
      made.positive_overflow = false;
      made.negative_overflow = false;
      made.positive_underflow = false;
      made.negative_underflow = false;
      return made;
    }
    if (hex[z] == '.') {
      parts_size = 0;
    } else {
    parts_size++;

    }
  }

  int stopping_point = 0;
  int afterDecimal = 0;
  int decimal1 = 0;
  int decimal2 = 0;
  int base = 1;
  int length = strlen(hex);
  if (hex[0] == '-') {
    stopping_point = 1;
  }
  
  for (int i = length--; i >= stopping_point; i--) {
    if (hex[i] >= '0' && hex[i] <= '9') {
      if (afterDecimal == 0) {
	decimal1 += (hex[i] - 48) * base;
      } else {
	decimal2 += (hex[i] - 48) * base;
      }
      base *= 16;
    } else if (hex[i] >= 'A' && hex [i] <= 'F') {
      if (afterDecimal == 0) {
	decimal1 += (hex[i] - 55) * base;
      } else {
	decimal2 += (hex[i] - 55) * base;
      }
      base *= 16;
    } else if (hex[i] >= 'a' && hex[i] <= 'b') {
      if (afterDecimal == 0) {
	decimal1 += (hex[i] - 87) *  base;
      } else {
	decimal2 += (hex[i] - 87) * base;
      }
      base *= 16;
    } else if (hex[i] == '.') {
      afterDecimal = 1;
      base = 1;
    }
  }
  
  made.integer1 = decimal2;
  made.integer2 = decimal1;
  if (stopping_point == 1) {
    made.valid_nonnegative = false;
    made.valid_negative = true;
  } else {
  made.valid_nonnegative = true;
  made.valid_negative = false;
  }
  made.error = false;
  made.positive_overflow = false;
  made.negative_overflow = false;
  made.positive_underflow = false;
  made.negative_underflow = false;
  
  return made;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.integer1;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.integer2;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  Fixedpoint made;
  uint64_t carry = 0;
  uint64_t frac_result;
  uint64_t whole_result;
  if ((left.valid_nonnegative && right.valid_nonnegative) || (left.valid_negative && right.valid_negative)) {
    frac_result = left.integer2 + right.integer2;
    if (frac_result < left.integer2) {
      carry = 1;
    }
    whole_result = left.integer1 + right.integer1 + carry;
    if (whole_result < left.integer1) {
      if (left.valid_nonnegative && right.valid_nonnegative) {
	made.positive_overflow = true;
	made.negative_overflow = false;
      } else {
	made.negative_overflow = true;
	made.positive_overflow = false;
      }
    } else  {
      made.positive_overflow = false;
      made.negative_overflow = false;
    }
    
    made.integer1 = whole_result;
    made.integer2 = frac_result;
    if (left.valid_nonnegative && right.valid_nonnegative) {
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else {
      made.valid_nonnegative = false;
      made.valid_negative = true;
    }
    made.error = false;
    made.positive_underflow = false;
    made.negative_underflow = false;
    
  } else if (left.valid_nonnegative && right.valid_negative) {
    return fixedpoint_sub(left, right);
  }
  
  return made;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  Fixedpoint made;
  uint64_t frac_result;
  uint64_t whole_result;
  
  
  if (left.valid_nonnegative && right.valid_nonnegative) {
    if ((left.integer1 > right.integer1) && (left.integer2 > right.integer2)) {
      frac_result = left.integer2 - right.integer2;
      whole_result = left.integer1 - right.integer1;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else if ((left.integer1 > right.integer1) && (left.integer2 < right.integer2)) {
      whole_result = left.integer1 - right.integer1 - 1;
      frac_result = left.integer2 - right.integer2 + 18446744073709551615 + 1;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else if ((left.integer1 < right.integer1) && (left.integer2 < right.integer2)) {
      frac_result = right.integer2 - left.integer2;
      whole_result = right.integer1 - left.integer1;
      made.valid_nonnegative = false;
      made.valid_negative = true;
    } else {
      frac_result = left.integer2 - right.integer2;
      whole_result = right.integer1 - left.integer1;
      made.valid_nonnegative = false;
      made.valid_negative = false;
    }
  } else if (left.valid_nonnegative && right.valid_negative) {
    right.valid_negative = false;
    right.valid_nonnegative = true;
    return fixedpoint_add(left, right);
  } else if (left.valid_negative && right.valid_nonnegative) {
    left.valid_nonnegative = true;
    left.valid_negative = false;
    made = fixedpoint_add(left, right);
    return fixedpoint_negate(made);
  } else {
    if ((left.integer1 > right.integer2) && (left.integer2 > right.integer2)) {
      frac_result = left.integer2 - right.integer2;
      whole_result = left.integer1 - right.integer1;
      made.valid_nonnegative = false;
      made.valid_negative = true;
    } else if ((left.integer1 > right.integer1) && (left.integer2 < right.integer2)) {
      whole_result = left.integer1 - right.integer1 - 1;
      frac_result = left.integer2 - right.integer2 + 18446744073709551615 + 1;
      made.valid_nonnegative = false;
      made.valid_negative = true;
    } else if ((left.integer1 < right.integer1) && (left.integer2 < right.integer2)) {
      frac_result = right.integer2 - left.integer2;
      whole_result = right.integer1 - left.integer1;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else {
      frac_result = left.integer2 - right.integer2;
      whole_result = right.integer1 - left.integer1;
      made.valid_nonegative = true;
      made.valid_negative = false;
    }
  }
  
  made.error = false;
  made.positive_overflow = false;
  made.negative_overflow = false;
  made.positive_underflow = false;
  made.negative_underflow = false;
  return made;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (fixedpoint_is_zero(val) == 1) {
    return val;
  }
  val.valid_nonnegative = false;
  val.valid_negative = true;
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  fixedpoint made;
  uint64_t whole_halved_val;
  uint64_t frac_havled_val;
  if ((val.integer2 % 2) == 0) {
    if ((val.integer1 % 2) == 0) {
      whole_halved_val = val.integer1 / 2;
      frac_halved_val = val.integer2 / 2;
    } else {
      whole_halved_val = val.integer1 / 2;
      frac_halved_val = (val.integer2 / 2) + ((18446744073709551615 + 1) / 2);
    }
    if (val.valid_nonnegative) {
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else {
      made.valid_nonnegative = false;
      made.valid_negative = true;
    }
    made.positive_underflow = false;
    made.negative_underflow = false;
    
  } else {
    if ((val.integer1 % 2) == 0) {
      whole_halved_val = val.integer1 / 2;
      frac_halved_val = val.integer2 / 2;
    } else {
      whole_halved_val = val.integer1 / 2;
      frac_halved_val = (val.integer2 / 2) + ((18446744073709551615 + 1) / 2);
    }
    if (val.valid_nonnegative) {
      made.valid_nonnegative = true;
      made.valid_negative = false;
      made.positive_underflow = true;
      made.negative_underflow = false;
    } else {
      made.valid_nonnegative = false;
      made.valid_negative = true;
      made.positive_underflow = false;
      made.negative_underflow = true;
    }
  }

  made.error = false;
  made.positive_overflow = false;
  made.negative_overflow = false;
  
  return made;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  fixedpoint made;
  uint64_t doubled_whole;
  uint64_t doubled_frac;
  short carry = 0;

  doubled_frac = val.integer2 * 2;
  if (doubled_frac < val.integer2) {
    carry = 1;
  }
  doubled_whole = val.integer1 * 2;
  if (carry = 1) {
    doubled_whole++;
  }

  if (doubled_whole < val.integer1) {
    if (val.valid_nonnegative) {
      made.valid_nonnegative = true;
      made.valid_negative = false;
      made.positive_overflow = true;
      made.negative_overflow = false;
    } else {
      made.valid_nonnegative = false;
      made.valid_negative = true;
      made.positive_overflow = false;
      made.negative_overflow = true;
    }
  } else  {
    made.positive_overflow = false;
    made.negative_overflow = false;
    if (val.valid_nonnegative) {
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else  {
      made.valid_nonnegative = false;
      made.valid_negative = true;
    }
  }

  made.integer1 = doubled_whole;
  made.integer2 = doubled_frac;
  made.error = false;
  made.positive_underflow = false;
  made.negative_underflow = false;
  
  return made;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  int response;

  if (left.valid_nonnegative && right.valid_nonnegative) {
    if (left.integer1 < right.integer1) {
      response = -1;
    } else if (left.integer1 > right.integer1) {
      response = 1;
    } else {
      if (left.integer2 < right.integer2) {
	response = -1;
      } else if (left.integer2 > right.integer2) {
	response = 1;
      } else {
	response = 0;
      }
    }
  } else if (left.valid_negative && right.valid_negative) {
    if (left.integer1 < right.integer1) {
      response = 1;
    } else if (left.integer1 > right.integer1) {
      response = -1;
    } else {
      if (left.integer2 < right.integer2) {
	response = 1;
      } else if (left.integer2 > right.integer2) {
	response = -1;
      } else {
	response = 0;
      }
    }
  } else if (left.valid_negative && right.valid_nonnegative) {
    response = -1;
  } else {
    response = 1;
  }
  
  return response;
}

int fixedpoint_is_zero(Fixedpoint val) {
  // TODO: implement
  if (val.valid_negative == false && (val.integer1 != 0 || val.integer2 != 0)) {
    return 0;
  }
  return 1;
}

int fixedpoint_is_err(Fixedpoint val) {
  if (val.error == true) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  if (val.valid_negative == true) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  if (val.negative_overflow == true) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if (val.positive_overflow == true) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if (val.negative_underflow == true) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if (val.positive_underflow == true) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  if (val.valid_nonnegative == true || val.valid_negative) {
    return 1;
  }
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  
  assert(0);
  char *s = malloc(20);
  int starting_point = 0;
  int spots_available_whole = 0;
  int spots_available_frac = 0;
  if (val.valid_negative) {
    s[0] = '-';
    starting_point = 1;
  }
  if (val.integer2 != 0) {
    if (starting_point == 1) {
      spots_available_whole = 9;
      spots_available_frac = 9;
    } else {
      spots_available_whole = 10;
      spots_available_frac = 9;
    }
  } else {
    if (starting_point == 1) {
      spots_available_whole = 19; 
      spots_available_frac = 0;
    } else {
      spots_available_whole = 20;
      spots_available_frac = 0;
    }
  }

  
  for (int i = starting_point; i < 20; i++) {
    
  }
  
  return s;
}
