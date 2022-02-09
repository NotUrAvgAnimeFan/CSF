#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <inttypes.h>

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

  //create error fixedpoint value for invalid hex string
  if (strlen(hex) == 0 || (strlen(hex) == 1 && (hex[0] == '-' || hex[0] == '.'))) {
    made.error = true;
    made.integer1 = 1;
    made.integer2 = 1;
    made.valid_nonnegative = true;
    made.valid_negative = false;
    made.positive_overflow = false;
    made.negative_overflow = false;
    made.positive_underflow = false;
    made.negative_underflow = false;
    free(whole);
    free(fraction);
    return made;
  }

  //determine the sign of the fixedpoint value to be created
  if (hex[starting_point] == '-') {
    made.valid_negative = true;
    made.valid_nonnegative = false;
    starting_point++;
  } else {
    made.valid_nonnegative = true;
    made.valid_negative = false;
  }

  int is_digit;
  unsigned int i = 0;
  
  //check if whole part of hex has 16 or less valid hex digits, create error fixedpoint if not
  while (i < 17) {
    if (hex[i + starting_point] == '.') {
      break;
    } else if ((i + starting_point) == strlen(hex)) {
      made.integer1 = strtoul(whole, NULL, 16);
      made.integer2 = 0;
      made.error = false;
      made.positive_overflow = false;
      made.negative_overflow = false;
      made.positive_underflow = false;
      made.negative_underflow = false;
      free(whole);
      free(fraction);
      return made;
    } else {
      if (i < 16) {
	is_digit = isxdigit(hex[i + starting_point]);
      } else {
	is_digit = 0;
      }
      if (is_digit == 0) {
	made.integer1 = strtoul(whole, NULL, 16);
	made.integer2 = 0;
	made.error = true;
	made.positive_overflow = false;
	made.negative_overflow = false;
	made.positive_underflow = false;
	made.negative_underflow = false;
	free(whole);
	free(fraction);
	return made;
      }
      
      whole[i] = hex[i + starting_point];
      i++;
      
    }
  }

  
  i++;
  
  //convert the whole part hex digits to a numeric value and store it
  made.integer1 = strtoul(whole, NULL, 16);

  //if the fraction part of the hex string is longer than 16 hex digits, create an error fixedpoint
  int size = strlen(hex) - (i + starting_point);

  
  if (size > 16) {
    made.integer2 = 1;
    made.error = true;
    made.positive_overflow = false;
    made.negative_overflow = false;
    made.positive_underflow = false;
    made.negative_underflow = false;
    free(whole);
    free(fraction);
    return made;
  }

  //if the fraction part of the hex string has invalid hex digits, create an error fixedpoint
  for (int j = 0; j < 16; j++) {
    //pad with zeros if fraction part is less than 16 digits
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
	free(whole);
	free(fraction);
	return made;
      }
      
      fraction[j] = hex[j + i + starting_point];

    }
  }

  //convert the fraction part hex digits to a numeric value and store it
  made.integer2 = strtoul(fraction, NULL, 16);

  //zero fixedpoint case
  if (made.integer1 == 0 && made.integer2 == 0) {
    made.valid_negative = false;
    made.valid_nonnegative = true;
  }

  made.error = false;
  made.positive_overflow = false;
  made.negative_overflow = false;
  made.positive_underflow = false;
  made.negative_underflow = false;
  free(whole);
  free(fraction);
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
	
  //compute addition when both fixedpoint operands have the same sign
  if ((left.valid_nonnegative && right.valid_nonnegative) || (left.valid_negative && right.valid_negative)) {
    //check if carrying is necessary for the fraction part
    frac_result = left.integer2 + right.integer2;
    if (frac_result < left.integer2) {
      carry = 1;
    }
    //if carrying is necessary for the whole part, overflow occurs
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
    //assign whole part, fraction part, and sign of resulting fixedpoint
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
    
  } else if (left.valid_negative && right.valid_nonnegative) {	//use subtraction for negative left operand and positive right operand
    left.valid_nonnegative = true;
    left.valid_negative = false;
    
    return fixedpoint_sub(right, left);

  } else if (left.valid_nonnegative && right.valid_negative) {  //use subtraction for negative right operand and positive left operand
    right.valid_nonnegative = true;
    right.valid_negative = false;
    return fixedpoint_sub(left, right);
  }
  
  return made;
}


/*
Fixedpoint sub_both_nonnegative(Fixedpoint left, Fixedpoint right) {
  uint64_t frac_result;
  uint64_t whole_result;

  if ()


  

  return fixedpoint_create2(whole_result, frac_result);
}


*/

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  Fixedpoint made;
  uint64_t frac_result;
  uint64_t whole_result;
  
  //compute subtraction when both fixedpoint operands are nonnegative
  if (left.valid_nonnegative && right.valid_nonnegative) {
    //both operands are equal therefore the result is zero  
    if (left.integer1 == right.integer1 && left.integer2 == right.integer2) {
      whole_result = 0;
      frac_result = 0;
      made.valid_nonnegative = true;
      made.valid_negative = false;

      //if left operand's whole and fraction parts are both biger than the right operand's, simple subtraction
    } else if ((left.integer1 > right.integer1) && (left.integer2 > right.integer2)) {
      frac_result = left.integer2 - right.integer2;
      whole_result = left.integer1 - right.integer1;
      made.valid_nonnegative = true;
      made.valid_negative = false;
	    
      //if left operand's whole part is bigger but fraction part is smaller, borrow  
    } else if ((left.integer1 > right.integer1) && (left.integer2 < right.integer2)) {
      whole_result = left.integer1 - right.integer1 - 1;
      frac_result = 18446744073709551615 - (right.integer2 - left.integer2) + 1;
      made.valid_nonnegative = true;
      made.valid_negative = false;
	    
      //if left operand's whole and fraction parts are both smaller, subtract left from right then negate  
    } else if ((left.integer1 < right.integer1) && (left.integer2 < right.integer2)) {
      frac_result = right.integer2 - left.integer2;
      whole_result = right.integer1 - left.integer1;
      made.valid_nonnegative = false;
      made.valid_negative = true;
	    
      //if left operand's whole part is smaller but fraction part is bigger, 
      //flipped subtraction and negation for whole part and simple subtraction for fraction part
    } else if ((left.integer1 < right.integer1) && (left.integer2 > right.integer2)){
      whole_result = right.integer1 - left.integer1 - 1;
      frac_result = 18446744073709551615 - (left.integer2 - right.integer2) + 1;
      made.valid_nonnegative = false;
      made.valid_negative = true;
	    
      //if left operand's whole part is smaller but fraction parts are equal, flipped subtraction and negation for whole part
    } else if (left.integer1 < right.integer1 && left.integer2 == right.integer2) {
      whole_result = right.integer1 - left.integer1;
      frac_result = 0;
      made.valid_negative = true;
      made.valid_nonnegative = false;
	    
      //if left operand's whole part is bigger but fraction parts are equal, simple subtraction for whole part
    } else if (left.integer1 > right.integer1 && left.integer2 == right.integer2) {
      whole_result = left.integer1 - right.integer1;
      frac_result = 0;
      made.valid_nonnegative = true;
      made.valid_negative = false;
	    
      //both operands are decimals with left operand's fraction part smaller, flipped subtraction and negation  
    } else if (left.integer2 < right.integer2) {
      whole_result = 0;
      frac_result = right.integer2 - left.integer2;
      made.valid_negative = true;
      made.valid_nonnegative = false;
      
      //both operands are decimals with left operand's fraction part bigger, simple subtraction
    } else if (left.integer2 > right.integer2) {
      whole_result = 0;
      frac_result = left.integer2 - right.integer2;
      made.valid_nonnegative = true;
      made.valid_negative = false;
    }
    
    //add when positive left operand and negative right operand 
  } else if (left.valid_nonnegative && right.valid_negative) {
    frac_result = left.integer2 + right.integer2;
    whole_result = left.integer1 + right.integer1;
    if (frac_result < left.integer2) {
      whole_result++;
    }
    if (whole_result < left.integer1) {
      made.positive_overflow = true;
      made.negative_overflow = false;
    }
    made.valid_nonnegative = true;
    made.valid_negative = false;
    made.integer1 = whole_result;
    made.integer2 = frac_result;
    made.positive_underflow = false;
    made.negative_underflow = false;
    
    return made;
        
    //add as nonnegatives and negate result when negative left operand and positive right operand 
  } else if (left.valid_negative && right.valid_nonnegative) {
    frac_result = left.integer2 + right.integer2;
    whole_result = left.integer1 + right.integer1;
    if (frac_result < left.integer2) {
      whole_result++;
    }
    if (whole_result < left.integer1) {
      made.negative_overflow = true;
      made.positive_overflow = false;
    }
    made.valid_negative = true;
    made.valid_nonnegative = false;
    made.integer1 = whole_result;
    made.integer2 = frac_result;
    made.positive_underflow = false;
    made.negative_underflow = false;
    return made;
    //both operands negative
  } else {
    
    return fixedpoint_sub(fixedpoint_negate(right), fixedpoint_negate(left));
    
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
	
  //0 fixedpoint negated is 0
  if (val.integer1 == 0 && val.integer2 == 0) {
    return val;
  }

  if (val.valid_nonnegative == true) {
    val.valid_nonnegative = false;
    val.valid_negative = true;
  } else  {
    val.valid_nonnegative = true;
    val.valid_negative = false;
  }
  
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  
  Fixedpoint made;
  short carry_decimals = 0;
	
  //halve the whole part
  made.integer1 = val.integer1 >> 1;
	
  //need to carry whole part if not evenly divisible by 2
  if (val.integer1 % 2 == 1) {
    carry_decimals = 1;
  }
  
  //halve the fraction part
  made.integer2 = val.integer2 >> 1;
	
  //if fraction part not evenly divisible by 2, underflow occurs 
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
  
  //carry
  if (carry_decimals == 1) {
    made.integer2 += 9223372036854775808;
  }
	
  //determine sign
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
  uint64_t max = 9223372036854775808;
  
  //double
  made.integer1 = val.integer1 << 1;
  made.integer2 = val.integer2 << 1;

  //if whole part is bigger than can be represented, overflow occurs
  if (val.integer1 >= max) {
    made.integer1 = 0;
    made.integer2 = 0;
    if (val.valid_nonnegative) {
      made.positive_overflow = true;
      made.negative_overflow = false;
    } else {
      made.negative_overflow = true;
      made.positive_overflow = false;
    }
    made.valid_nonnegative = false;
    made.valid_negative = false;
    made.error = false;
    made.positive_underflow = false;
    made.negative_underflow = false;
    return made;
  }


  //carry
  if (val.integer2 >= max) {
    made.integer1++;
  }



  //determine sign
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
  //both positive	
  if (left.valid_nonnegative && right.valid_nonnegative) {
    //right whole part bigger
    if (left.integer1 < right.integer1) {
      response = -1;
	    
    //left whole part bigger
    } else if (left.integer1 > right.integer1) {
      response = 1;
	    
    //whole parts equal
    } else {
      //right fraction bigger
      if (left.integer2 < right.integer2) {
	response = -1;
	      
      //left fraction bigger
      } else if (left.integer2 > right.integer2) {
	response = 1;
	      
      //left and right equal
      } else {
	response = 0;
      }
    }
	  
  //both negative
  } else if (left.valid_negative && right.valid_negative) {
	  
    //right whole part bigger
    if (left.integer1 < right.integer1) {
      response = 1;
	    
    //left whole part bigger
    } else if (left.integer1 > right.integer1) {
      response = -1;
	    
    //whole parts equal
    } else {
      //right fraction bigger
      if (left.integer2 < right.integer2) {
	response = 1;
	      
      //left fraction bigger
      } else if (left.integer2 > right.integer2) {
	response = -1;
	      
      //left and right equal
      } else {
	response = 0;
      }
    }
	  
  //left negative, right positive
  } else if (left.valid_negative && right.valid_nonnegative) {
    response = -1;
	  
  //right negative, left positive
  } else {
    response = 1;
  }
  
  return response;
}

int fixedpoint_is_zero(Fixedpoint val) {
  if (val.integer1 == 0 && val.integer2 == 0) {
    return 1;
  }
  return 0;
}

int fixedpoint_is_err(Fixedpoint val) {
  if (val.error) {
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
  
  //determine sign
  if (val.valid_negative) {
    s[0] = '-';
    offset++;
  }
  
  //convert whole part numeric values to 16 hex digits and pad left side with zeros
  sprintf(whole, "%016lx", val.integer1); 
	
  //add whole part hex digits to final hex string without the padding
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
  
  //no fraction part
  if (val.integer2 == 0) {
    s[offset] = '\0';
    free(whole);
    free(fraction);
    return s;
  }
 
  s[offset] = '.';
  offset++;
	
  //convert fraction part numeric values to 16 hex digits and pad left side with zeros
  sprintf(fraction, "%016lx", val.integer2);
	
  //cut off fraction part where padding starts
  for (int z = 15; z >= 0; z--) {
    if (fraction[z] == '0') {
      fraction[z] = '\0';
    } else {
      break;
    }
  }

  //add fraction part hex digits to final hex string
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
