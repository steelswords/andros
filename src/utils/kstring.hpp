#ifndef _ANDROS_KSTRING_HPP
#define _ANDROS_KSTRING_HPP

#ifndef __UNIT_TEST__
#include <stdbool.h>
#include <stddef.h> //size_t
#endif

#define KSTRING_DEFAULT_SIZE 24
#define KSTRING_MAX_SIZE 

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

  /* Construct a string from an exisiting string. */
  kstring(char str[], int len);

  /* Static string helper functions */
  static bool isCharDigit(char c);
  static size_t strLength(const char* str);
  static int numDigitsInInt(int value);

  /* Methods */
  int toInt();
  size_t length();

  size_t m_len;
  char m_data[];

  static char* kernelStringMemory;
  static char* kernelStringMemoryEnd;
protected:
  static bool kernelStringMemoryInitialized;
  
  /* Takes care of allocating kernel strings in special region */
  void allocateMemory();
};

#endif
