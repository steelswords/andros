#ifndef _ANDROS_KSTRING_HPP
#define _ANDROS_KSTRING_HPP

#ifndef __UNIT_TEST__
#include <stdbool.h>
#include <stddef.h> //size_t
#include <stdint.h>
#endif

#define KSTRING_DEFAULT_SIZE 24
#define KSTRING_MAX_SIZE 

//TODO: + operator
//TODO: Copy constructor
//TODO: equality checking
//TODO: substring ops

// Pointers to kernel-space string storage.
// Set up in boot.s
extern "C" char* kstring_area_begin_ptr;
extern "C" char* kstring_area_end_ptr;

/* A string class only to be used in the kernel.
 * Thus, most members are public.
 */
class kstring
{
public:
  
  // Constructors
  kstring();

  /* Construct a string representation of an integer. */
  kstring(int value);

  /* Construct a string representation of an integer in a given base. */
  kstring(int value, int base);

  /* Construct a string representation of an integer in a given base. */
  kstring(uint32_t value, int base);

  /* Construct a string from an exisiting string. */
  /* len is including the terminating null character */
  kstring(const char str[], const int len);

  /* Or if the length is not given, deduce it by looking for a null character */
  kstring(const char str[]);

  /* Static string helper functions */
  static bool isCharDigit(char c);
  static size_t strLength(const char* str);

  /* Caveat: destination shall be large enough to accomodate. */
  static void copyString(char* destination, const char* source);
  static int numDigitsInInt(int value);

  /* Methods */
  int toInt();

  /* Returns the number of non-null characters from the start
   * of m_data. Does not necessarily return the amount of 
   * memory allocated for this kstring. */
  size_t length();

  char* m_data;

  static bool  trimHexValues;
  static char* kernelStringMemory;
  static char* kernelStringMemoryEnd;
  static char* kernelStringAllocationError;
  static char* kernelStringMemoryIndex;
  static bool kernelStringMemoryInitialized;
  static int debugVal;
  static char* debugLastStringAddr;
protected:
  
  size_t m_len;

  /* Takes care of allocating kernel strings in special region */
  void init(size_t sizeToAllocate);
};

#endif
