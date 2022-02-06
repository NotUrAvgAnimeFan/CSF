#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

Fixedpoint fixedpoint_create(uint64_t whole) {
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
  char *whole = malloc(16);
  char *fraction = malloc(16);
  int starting_point = 0;
  Fixedpoint made;

  if (hex[starting_point] == '-') {
    made.valid_negative = true;
    made.valid_nonnegative = false;
    starting_point++;
  } else {
    made.valid_nonnegative = true;
    made.valid_negative = false;
  }

  int is_digit;
  int i = 0;
  while (hex[i + starting_point] != '.') {
    is_digit = isxdigit(hex[i + starting_point]);
    if (i > 15 || is_digit == 0) {
      made.integer1 = strtoul(whole, NULL, 16);
      made.integer2 = 0;
      made.error = true;
      made.positive_overflow = false;
      made.negative_overflow = false;
      made.positive_underflow = false;
      made.negative_underflow = false;
      return made;
    }
    whole[i] = hex[i + starting_point];
    i++; 
  }
  
  i++;

  made.integer1 = strtoul(whole, NULL, 16);

  int size = strlen(hex) - (i + starting_point);
  if (size > 16) {
    made.integer2 = 1;
    made.error = true;
    made.positive_overflow = false;
    made.negative_overflow = false;
    made.positive_underflow = false;
    made.negative_underflow = false;
    return made;
  }
  
  for (int j = 0; j < 16; j++) {
    
    if (j >= size) {
      fraction[j] = '0';
    } else {
      is_digit = isxdigit(hex[j + i + starting_point]);
      if (is_digit == 0) {
	made.integer2 = 1;
	made.error = true;
	made.positive_overflow = false;
	made.negative_overflow = false;
	made.positive_underflow = false;
	made.negative_underflow = false;
	return made;
      }
      
      fraction[j] = hex[j + i + starting_point];
    }
  }
  
  made.integer2 = strtoul(fraction, NULL, 16);
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
    
  } else if (left.valid_negative && right.valid_nonnegative) {
    left.valid_nonnegative = true;
    left.valid_negative = false;
    
    return fixedpoint_sub(right, left);

  } else if (left.valid_nonnegative && right.valid_negative) {
    right.valid_nonnegative = true;
    right.valid_negative = false;
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
    } else if ((left.integer1 < right.integer1) && (left.integer2 > right.integer2)){
      frac_result = left.integer2 - right.integer2;
      whole_result = right.integer1 - left.integer1;
      made.valid_nonnegative = false;
      made.valid_negative = true;
    } else if (left.integer1 < right.integer1 && left.integer2 == right.integer2) {
      whole_result = right.integer1 - left.integer1;
      frac_result = 0;
      made.valid_negative = true;
      made.valid_nonnegative = false;
    } else if (left.integer1 > right.integer1 && left.integer2 == right.integer2) {
      whole_result = left.integer1 - right.integer1;
      frac_result = 0;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else if (left.integer2 < right.integer2) {
      whole_result = 0;
      frac_result = right.integer2 - left.integer2;
      made.valid_negative = true;
      made.valid_nonnegative = false;
    } else if (left.integer2 > right.integer2) {
      whole_result = 0;
      frac_result = left.integer2 - right.integer2;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else {
      whole_result = 0;
      frac_result = 0;
      made.valid_nonnegative = true;
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
    if ((left.integer1 > right.integer1) && (left.integer2 > right.integer2)) {
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
    } else if ((left.integer1 < right.integer1) && (left.integer2 > right.integer2)) {
      frac_result = left.integer2 - right.integer2;
      whole_result = right.integer1 - left.integer1;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else if (left.integer1 < right.integer1 && left.integer2 == right.integer2) {
      whole_result = right.integer1 - left.integer1;
      frac_result = 0;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else if (left.integer1 > right.integer1 && left.integer2 == right.integer2) {
      whole_result = left.integer1 - right.integer1;
      frac_result = 0;
      made.valid_negative = true;
      made.valid_nonnegative = false;
    } else if (left.integer1 == right.integer1 && left.integer2 < right.integer2) {
      whole_result = 0;
      frac_result = right.integer2 - left.integer2;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    } else if (left.integer1 == right.integer1 && left.integer2 > right.integer2) {
      whole_result = 0;
      frac_result = left.integer2 - right.integer2;
      made.valid_negative = true;
      made.valid_nonnegative = false;
    } else {
      whole_result = 0;
      frac_result = 0;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    }
    
  }

  made.integer1 = whole_result;
  made.integer2 = frac_result;
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

  val.valid_nonnegative = !(val.valid_nonnegative);
  val.valid_negative = !(val.valid_negative);
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  
  Fixedpoint made;
  short carry_decimals = 0;
  made.integer1 = val.integer1 >> 1;
  if (val.integer1 % 2 == 1) {
    carry_decimals = 1;
  }
  
  made.integer2 = val.integer2 >> 1;
  if (val.integer2 % 2 == 1) {
    if (val.valid_nonnegative) {
      made.valid_nonnegative = true;
      made.valid_negative = false;
      made.positive_underflow = true;
      made.negative_underflow = false;
    } else {
      made.valid_negative = true;
      made.valid_nonnegative = false;
      made.negative_underflow = true;
      made.positive_underflow = false;
    }
    
    made.error = false;
    made.positive_overflow = false;
    made.negative_overflow = false;
    return made;
  }
  
  if (carry_decimals == 1) {
    made.integer2 += 1152921504606846976;
  }
  if (val.valid_nonnegative) {
    made.valid_nonnegative = true;
    made.valid_negative = false;
  } else {
    made.valid_negative = true;
    made.valid_nonnegative = false;
  }
  made.error = false;
  made.positive_overflow = false;
  made.negative_overflow = false;
  made.positive_underflow = false;
  made.negative_underflow = false;

  return made;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  Fixedpoint made;
  
  made.integer1 = val.integer1 << 1;
  if (val.integer1 >= 1152921504606846976) {
    made.integer2 = val.integer2 << 1;
    if (val.valid_nonnegative) {
      made.valid_nonnegative = true;
      made.valid_negative = false;
      made.positive_overflow = true;
      made.negative_overflow = false;
    } else {
      made.valid_negative = true;
      made.valid_nonnegative = false;
      made.negative_overflow = true;
      made.positive_overflow = false;
    }
    made.error = false;
    made.positive_underflow = false;
    made.negative_underflow = false;    
  }

  made.integer2 = val.integer2 << 1;
  if (val.integer2 >= 1152921504606846976) {
    made.integer1++;
  }

  if (val.valid_nonnegative) {
    made.valid_nonnegative = true;
    made.valid_negative = false;
  } else {
    made.valid_negative = true;
    made.valid_nonnegative = false;
  }

  made.positive_overflow = false;
  made.negative_overflow = false;
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
  if (val.valid_negative == false && (val.integer1 != 0 || val.integer2 != 0)) {
    return 0;
  }
  return 1;
}

int fixedpoint_is_err(Fixedpoint val) {
  if (val.error) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  if (val.valid_negative) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  if (val.negative_overflow) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  if (val.positive_overflow) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  if (val.negative_underflow) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  if (val.positive_underflow) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  if (val.valid_nonnegative || val.valid_negative) {
    return 1;
  }
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  
  char *s = malloc(34);
  char *whole = malloc(16);
  char *fraction = malloc(16);
  int offset = 0;
  
  if (val.valid_negative) {
    s[0] = '-';
    offset++;
  }
  
  sprintf(whole, "%016lx", val.integer1); 
  int i = 0;
  while (i < 15) {
    if (whole[i] != '0') {
      s[offset] = whole[i];
      offset++;
    }
    i++;
  }

  s[offset] = whole[i];
  offset++;
  
  if (val.integer2 == 0) {
    s[offset] = '\0';
    free(whole);
    free(fraction);
    return s;
  }
 
  s[offset] = '.';
  offset++;
  sprintf(fraction, "%016lx", val.integer2);
  
  for (int z = 15; z >= 0; z--) {
    if (fraction[z] == '0') {
      fraction[z] = '\0';
    } else {
      break;
    }
    
  }

  
  int j = 0;
  while (j < 16) {
    
    s[offset] = fraction[j];
    offset++;
    j++;
  }
  s[offset] = '\0';

  free(whole);
  free(fraction);
  return s;
}
