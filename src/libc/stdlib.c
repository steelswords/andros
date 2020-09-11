#ifndef __UNIT_TEST__
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#endif


#ifdef __UNIT_TEST__
#include <string.h>
#ifdef _DEBUG_
#include <iostream>
#endif
#endif

inline bool isDigit(char c)
{
  if (c >= 0x30 && c <= 0x39) return true;
  else return false;
}

int numDigits(int i)
{
  if (i < -999999999 || i > 999999999) return 10;
  if (i < -99999999  || i > 99999999)  return 9;
  if (i < -9999999   || i >  9999999)  return 8;
  if (i < -999999    || i >   999999)  return 7;
  if (i < -99999     || i >    99999)  return 6;
  if (i < -9999      || i >     9999)  return 5;
  if (i < -999       || i >      999)  return 4;
  if (i < -99        || i >       99)  return 3;
  if (i < -9         || i >        9)  return 2;
  return 1;
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
  int j = 0;
  while(curDigit >= 0)
  {
    value += digits[j++] * multipliers[--curDigit];
  }

  if (!positive)
    value *= (long)-1;

  return (int)value;
 
}

void itoa(int value, char* str)
{
  bool negative = (value < 0);
  int digits = numDigits(value);
  if (negative)
    value = -1 * value;
#ifdef __UNIT_TEST__
#ifdef _DEBUG_
  std::cout << "itoa(" << value << ") called " << ((negative) ? "(negative value)." : ".") << std::endl;
  std::cout << "Number of digits: " << digits << std::endl;
#endif
#endif


  if (negative)
  {
    digits ++;
  }

  str[digits] = '\0';
    
  while ( digits >= 0)
  {

#ifdef __UNIT_TEST__
#ifdef _DEBUG_
  std::cout << "Value of digit " << digits -1 << ": " << value % 10 << std::endl;
#endif
#endif
    str[digits - 1] = (value % 10) + 0x30;
    digits--;
    value /= 10;
  }  

  if (negative)
  {
    str[0] = '-';
  }

}

char itoaHexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                          '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

const char* itoaUnsupportedBase = "Bad Base";

void itoa(int value, char* str, int base)
{
  if(base == 10)
  {
    itoa(value, str);
  }
  else if (base == 16)
  {
    str[0] = '0';
    str[1] = 'x';
    if (value == 0)
    {
      str[2] = '0';
      str[3] = '\0';
      return;
    }
    int index = 2;
    int x = value;
    for (int shiftAmount = 28; shiftAmount >= 0; shiftAmount -= 4)
    {
      int hexDigit = (((unsigned int) x) >> shiftAmount) & 0xF;
      str[index] = itoaHexDigits[hexDigit];
      index++;
    }
  }
  else
  {
    // Copy error string into return value. 
    // Hurrah for not having strcpy implemented yet. 
    int i = 0;
    for (; i < strlen(itoaUnsupportedBase); ++i)
    {
      str[i] = itoaUnsupportedBase[i];
      i++;
    }
    str[i] = '\0';
  }
}
