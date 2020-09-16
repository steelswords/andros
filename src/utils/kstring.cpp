#include "kstring.hpp"
#ifdef __UNIT_TEST__
#include <iostream>

#else
#include <stddef.h>
#endif

bool kstring::kernelStringMemoryInitialized = false;
char* kstring::kernelStringMemory = kstring_area_begin;
char* kstring::kernelStringMemoryEnd = kstring_area_end;
char* kstring::kernelStringMemoryIndex = kstring_area_begin;

// Forward declarations
static void itoa(int value, char* str, int base);

kstring::kstring()
{
  init(KSTRING_DEFAULT_SIZE);
}

kstring::kstring(int value)
{
  init(12); // (-)2000000000
  itoa(value, m_data, 10);
}

kstring::kstring(int value, int base)
{
  init(12); // Ok to use extra bytes
  itoa(value, m_data, base);
}

kstring::kstring(char str[])
{
  m_len = kstring::strLength(str);
  init(m_len);
  kstring::copyString(m_data, str);
}

kstring::kstring(char str[], int len)
{
  init(len);
  kstring::copyString(m_data, str);
}

bool kstring::isCharDigit(char c)
{
  if (c >= 0x30 && c <= 0x39) return true;
  else return false;
}

int kstring::numDigitsInInt(int value)
{
  if (value < -999999999 || value > 999999999) return 10;
  if (value < -99999999  || value >  99999999) return 9;
  if (value < -9999999   || value >   9999999) return 8;
  if (value < -999999    || value >    999999) return 7;
  if (value < -99999     || value >     99999) return 6;
  if (value < -9999      || value >      9999) return 5;
  if (value < -999       || value >       999) return 4;
  if (value < -99        || value >        99) return 3;
  if (value < -9         || value >         9) return 2;
  return 1;
}

int kstring::toInt()
{
  // I know this is going to be pretty slow, but optimization
  // just isn't worth my time at this point here.
  // I know, it kills me too.

#ifdef __UNIT_TEST__
#ifdef _DEBUG_
  std::cout << "atoi(" << m_data << ") called." << std::endl;
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
  if (m_data[stringIndex] == '-')
  {
    positive = false;
    stringIndex++;
  }

  // *** Gather digits from string ***
  while(m_data[stringIndex] != '\0' && kstring::isCharDigit(m_data[stringIndex]) )
  {
    digits[curDigit] = (int)(m_data[stringIndex] - 0x30);
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
} /* toInt() */

static void itoa(int value, char* str, int base)
{
  char itoaHexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  const char* itoaUnsupportedBase = "Bad Base";

  if(base == 10)
  {
    bool negative = (value < 0);
    int digits = kstring::numDigitsInInt(value);
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
  else if (base == 16)
  {
    bool leadingDigitEncountered = false;
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
      if (hexDigit != 0)
        leadingDigitEncountered = true;
      if (hexDigit == 0 && !leadingDigitEncountered)
      {
      }
      else
      {
        str[index] = itoaHexDigits[hexDigit];
        index++;
      }
    }
  }
  else
  {
    // Copy error string into return value. 
    // Hurrah for not having strcpy implemented yet. 
    size_t i = 0;
    for (; i < kstring::strLength(itoaUnsupportedBase); ++i)
    {
      str[i] = itoaUnsupportedBase[i];
      i++;
    }
    str[i] = '\0';
  }
}

size_t kstring::strLength(const char* str)
{
  int len = 0;
  while (*str != '\0')
  {
    str++;
    ++len;
  }
  return len;
}

void kstring::copyString(char* destination, const char* source)
{
  size_t sourceEnd = kstring::strLength(source); 
  for (size_t i = 0; i < sourceEnd; ++i)
  {
    destination[i] = source[i];
  }
}

size_t kstring::length() 
{
  return kstring::strLength(m_data);
}


void kstring::init(size_t sizeToAllocate)
{
  if (!kstring::kernelStringMemoryInitialized) 
  {
    if ( (kstring_area_begin != 0) &&
         (kstring_area_end   != 0))
    {
      kstring::kernelStringMemory    = kstring_area_begin;
      kstring::kernelStringMemoryEnd = kstring_area_end;
      kstring::kernelStringMemoryIndex = 0;
      kstring::kernelStringMemoryInitialized = true;
    }
  }
  // Now that the memory is initialized, let's allocate some.
  // End actually points to the next thing. Don't overwrite that.
  if (kstring::kernelStringMemoryIndex + sizeToAllocate >= kstring::kernelStringMemoryEnd)
  {
    //TODO: Throw an exception
  }
  else
  {
    m_data = kstring::kernelStringMemoryIndex;
    //TODO: Align to something that makes sense
    kstring::kernelStringMemoryIndex += sizeToAllocate;
    m_len = sizeToAllocate;
    m_data[m_len - 1] = '\0'; //Set terminating null char.

  }
}
