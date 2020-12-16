#ifdef __UNIT_TEST__
#include <iostream>

#else
#include <stddef.h>
#include <stdint.h>
#include "kstring.hpp"
#endif

bool kstring::kernelStringMemoryInitialized = false;
bool kstring::trimHexValues = true;
int kstring::debugVal = 0;
char* debugLastStringAddr = 0;

#ifdef __UNIT_TEST__
size_t kstringMemAreaLength = 16384;
char* kstring::kernelStringMemory = (char*)malloc(sizeof(char) * kstringMemAreaLength);
char* kstring::kernelStringMemoryEnd = kstring::kernelStringMemory + kstringMemAreaLength;
char* kstring::kernelStringMemoryIndex = kstring::kernelStringMemory;

#else
char* kstring::kernelStringMemory = kstring_area_begin_ptr;
char* kstring::kernelStringMemoryEnd = kstring_area_end_ptr;
char* kstring::kernelStringMemoryIndex = kstring_area_begin_ptr;
#endif

char* kstring::kernelStringAllocationError = "ERROR ALLOCATING STRING";

char kstring_index_no_update_error[] = "This index isn't updating!";

// Forward declarations
static void itoa(int value, char* str, int base);
static void itoaHex64(uint64_t value, char* str);

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
  init(12); // It's ok to use extra bytes
  itoa(value, m_data, base);
}

kstring::kstring(uint32_t value, int base)
{
  init(12); // It's ok to use extra bytes
  itoa(value, m_data, base);
}

kstring::kstring(uint64_t hexValue)
{
  init(2 + 16 + 1);
  itoaHex64(hexValue, m_data);
}

kstring::kstring(const char str[])
{
  m_len = kstring::strLength(str);
  init(m_len);
  kstring::copyString(m_data, str);
}

kstring::kstring(const char str[], const int len)
{
  init(len);
  kstring::copyString(m_data, str, len);
}

bool kstring::isCharDigit(char c)
{
  if (c >= 0x30 && c <= 0x39) return true;
  else return false;
}

static bool isEqual(kstring& a, kstring& b)
{

  bool retVal = true;
  if (a.length() == b.length())
  {
    // Loop through each character, checking for correctness.
    for (int i = 0; i < a.length(); ++i)
    {
      if (a.m_data[i] != b.m_data[i])
      {
        retVal = false;
        break;
      }
    }
  }
  else
  {
    retVal = false;
  }
  return retVal;
}

bool kstring::isEqual(char* a, char* b)
{
  bool retVal = true;
  if (strLength(a) == strLength(b))
  {
    // Loop through each character, checking for correctness.
    for (int i = 0; i < strLength(a); ++i)
    {
      if (a[i] != b[i])
      {
        retVal = false;
        break;
      }
    }
  }
  else
  {
    retVal = false;
  }
  return retVal;
}

bool kstring::startsWith(char* stringToBeTested, char* beginning)
{
  int beginningSize = kstring::strLength(beginning);
  bool result = true;
  for (int i = 0; i < beginningSize; ++i)
  {
    if (stringToBeTested[i] != beginning[i])
    {
      result = false;
      break;
    }
  }
  return result;
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

  // *** Skip leading whitespace ***
  int indexWhereNumberStarts = 0;
  while (m_data[indexWhereNumberStarts] == ' '
      || m_data[indexWhereNumberStarts] == '\n'
      || m_data[indexWhereNumberStarts] == '\t')
  {
    indexWhereNumberStarts++;
  }
  stringIndex = indexWhereNumberStarts;
  
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

static bool isHexDigit(int i)
{
  if (i <= '9' && i >= '0')
    return true;
  else if (i <= 'f' && i >= 'a')
    return true;
  else
    return false;
}

static uint8_t interpretHexDigit(char a)
{
  if (a <= '9' && a >= '0')
  {
    return a - 0x30;
  }
  else if (a <= 'f' && a >= 'a')
  {
    return a - 0x61 + 10;
  }
  return 0;
}

int kstring::toIntFromHex(const char* str, size_t &lengthOfNumber)
{
  uint32_t workingValue = 0;
  lengthOfNumber = 0;

  //Skip through the string until a number is reached
  while (!isHexDigit(str[lengthOfNumber]))
  {
    lengthOfNumber++;
  }

  // Now, lengthOfNumber indexes the first number in the string.
  // Peek ahead and see if it's a 0x format, and if so, discard.
  if (str[lengthOfNumber] == '0' && str[lengthOfNumber + 1] == 'x' && str[lengthOfNumber] != '\0')
  {
    lengthOfNumber += 2;
  }

  while(str[lengthOfNumber] != '\0' && isHexDigit(str[lengthOfNumber]))
  {
    workingValue = workingValue << 4;
    workingValue += interpretHexDigit(str[lengthOfNumber]);
    lengthOfNumber++;
  } 

  return workingValue;
}

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
      if (hexDigit == 0 && !leadingDigitEncountered && kstring::trimHexValues)
      {
      }
      else
      {
        str[index] = itoaHexDigits[hexDigit];
        index++;
      }
    }
    str[index] = '\0';
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

static void itoaHex64(uint64_t value, char* str)
{
  const int lengthOf32bitHexString = 2 + 8 + 1;
  char buf[ lengthOf32bitHexString * 2] = {0};
  char* highString = &buf[0];
  char* lowString  = &buf[lengthOf32bitHexString]; 

  uint32_t lowPart = (uint32_t)(value & 0xFFFFFFFF);
  uint32_t highPart = (uint32_t)((value >> 32) & 0xFFFFFFFF);

  bool oldTrimSetting = kstring::trimHexValues;
  kstring::trimHexValues = false;
  itoa(highPart, highString, 16);
  itoa(lowPart,  lowString , 16);
  kstring::trimHexValues = oldTrimSetting;

  //Get rid of 0x in front of the strings
  highString += 2;
  lowString  += 2;

  int highStringLength = kstring::strLength(highString);
  int lowStringLength  = kstring::strLength(lowString);

  // Now, loop through the values and assemble the output string.
  int index = 0;
  int dstIndex = 0 + 2; // For "0x" to go in front.
  
  str[0] = '0';
  str[1] = 'x';

  //Copy the higher string
  for(int index = 0; index < highStringLength; ++index)
  {
    char curChar = highString[index];
    if (curChar == '\0') break;
    str[dstIndex] = curChar;
    dstIndex++;
  }

  //Copy the lower string
  for(int index = 0; index < lowStringLength; ++index)
  {
    char curChar = lowString[index];
    if (curChar == '\0') break;
    str[dstIndex] = curChar;
    dstIndex++;
  }

  //Add terminating null char.
  str[dstIndex] = '\0';
}

size_t kstring::strLength(const char* str)
{
  int len = 1;
  while (*str != '\0')
  {
    str++;
    len++;
  }
  return len;
}

void kstring::copyString(char* destination, const char* source)
{
  size_t sourceEnd = kstring::strLength(source); 
  kstring::copyString(destination, source, sourceEnd);
  
}

void kstring::copyString(char* destination, const char* source, size_t length)
{
  for (size_t i = 0; i < length; ++i)
  {
    destination[i] = source[i];
  }
}

size_t kstring::length() const
{
  return kstring::strLength(m_data);
}


/* Allocated memory in kernel space for the string.
 * @param sizeToAllocate Number of bytes in the string, including the
 * null terminating character.
 */
void kstring::init(size_t sizeToAllocate)
{
  // TODO: Wrap this in a static method
  // First, make sure static members are initialized
  if (!kstring::kernelStringMemoryInitialized) 
  {
#ifdef __UNIT_TEST__
#else
    if ( (kstring_area_begin_ptr != 0) &&
         (kstring_area_end_ptr   != 0))
    {
      kstring::kernelStringMemory    = kstring_area_begin_ptr;
      kstring::kernelStringMemoryEnd = kstring_area_end_ptr;
      kstring::kernelStringMemoryIndex = kstring_area_begin_ptr;
#endif
      kstring::kernelStringMemoryInitialized = true;
      kstring::kernelStringAllocationError = "ERROR ALLOCATING STRING";
#ifndef __UNIT_TEST__
    }
#endif
  }
  // Now that the memory is initialized, let's allocate some.
  // End actually points to the next thing. Don't overwrite that.
  if (kstring::kernelStringMemoryIndex + sizeToAllocate >= kstring::kernelStringMemoryEnd)
  {
    //TODO: Throw an exception
    m_data = kstring::kernelStringAllocationError;
    debugVal += 0x00000100;
  }
  else
  {
    m_data = kstring::kernelStringMemoryIndex;
    //TODO: Align to something that makes sense
    char* errorCheck = kstring::kernelStringMemoryIndex;
    kstring::kernelStringMemoryIndex += sizeToAllocate;
    if (kstring::kernelStringMemoryIndex == errorCheck)
    {
      kstring::debugVal = 6;
    }
    m_len = sizeToAllocate;
    m_data[m_len - 1] = '\0'; //Set terminating null char.

  }
}

/* Operators */
bool kstring::operator==(const kstring& other)
{
  //TODO: Consolidate with isEqual()
  bool retVal = true;
  if (other.length() == this->length())
  {
    // Loop through each character, checking for correctness.
    for (int i = 0; i < this->length(); ++i)
    {
      if (this->m_data[i] != other.m_data[i]) retVal = false;
    }
  }
  else
  {
    retVal = false;
  }
  return retVal;
}

bool kstring::operator!=(const kstring& other)
{
  return ! (this->operator==(other));
}

kstring kstring::operator+(const kstring& other)
{
  int totalLength = this->length() + other.length() + 1; //1 for terminating null
  kstring returnString(this->m_data, totalLength);
  char* endOfFirstHalf = this->m_data + this->length();
  kstring::copyString(endOfFirstHalf, other.m_data);
}
