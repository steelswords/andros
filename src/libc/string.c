#include "libc/include/string.h"
#ifndef __UNIT_TEST__
#define <stddef.h> //size_t
#endif

size_t strlen(const char* str)
{
  size_t len = 0;
  while (*str != '\0')
  {
    str++;
    ++len;
  }
  return len;
}
