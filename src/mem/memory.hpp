#ifndef _ANDROS_MEMORY_TOP_LEVEL_HEADER_
#define _ANDROS_MEMORY_TOP_LEVEL_HEADER_
#include "stddef.h"

void* malloc(size_t size);

inline void operator delete(void* target) {}
inline void operator delete[](void* target) {}

inline void* operator new(size_t size)
{
  return malloc(size);
}

inline void* operator new[](size_t size)
{
  return malloc(size);
}

inline void* operator new(size_t size, void* ptr)   { return ptr; }
inline void* operator new[](size_t size, void* ptr) { return ptr; }
#if 0
inline void* operator new(unsigned long size) { return malloc(size); }
inline void* operator new[](unsigned long size) { return malloc(size); }
#endif

#endif
