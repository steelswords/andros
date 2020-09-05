#ifndef __UNIT_TEST__
#include "stdlib.h"
#include "stdbool.h"
#endif

#ifdef __UNIT_TEST__
#ifdef _DEBUG_
#include <iostream>
#endif
#endif

inline bool isDigit(char c)
{
  if (c >= 0x30 && c <= 0x39) return true;
  else return false;
}

int atoi(const char* str)
{
  // I know this is going to be pretty slow, but optimization
  // just isn't worth my time at this point here.
  // I know, it kills me too.

#ifdef __UNIT_TEST__
#ifdef _DEBUG_
  std::cout << "atoi(" << str << ") called." << std::endl;
#endif
#endif
  
  // Max size of int = 2,147,483,647. We need 10 digits and
  // a +/- indicator
  bool positive = true;
  long value = 0;
  int valueLength = 0;
  long digits[10] = {0};
  int curDigit = 0;
  int stringIndex = 0;
  int multipliers[10] = {1, 10, 100, 1000, 10000, 100000,
                         1000000, 10000000, 100000000, 1000000000};

  //TODO: Skip whitespace
  // *** Skip leading whitespace ***
  
  // *** Parse +/- ***
  // Really, all we care about is if there is a negative.
  if (str[stringIndex] == '-')
  {
    positive = false;
    stringIndex++;
  }

  // *** Gather digits from string ***
  while(str[stringIndex] != '\0' && isDigit(str[stringIndex]) )
  {
    digits[curDigit] = (int)(str[stringIndex] - 0x30);
    curDigit++;
    stringIndex++;
  }
  
  // *** Assemble digits into value
  //for (int i = curDigit; i > 0; --i)
  
#ifdef __UNIT_TEST__
#ifdef _DEBUG_
  for (int i = 0; i < 10; ++i)
  {
    std::cout << "digits[" << i << "] = " << digits[i] << std::endl;
  }
#endif
#endif
  valueLength = curDigit;
  int j = 0;
  while(curDigit >= 0)
  {
    value += digits[j++] * multipliers[--curDigit];
  }

  if (!positive)
    value *= (long)-1;

  return (int)value;
 
}
