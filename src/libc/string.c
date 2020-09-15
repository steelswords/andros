#ifndef __UNIT_TEST__
//#include <stddef.h> //size_t
#include "string.h"
#endif

int kstrlen(const char* str)
{
  int len = 0;
  while (*str != '\0')
  {
    str++;
    ++len;
  }
  return len;
}
