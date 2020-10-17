#ifndef _ANDROS_MEMORY_TOP_LEVEL_HEADER_
#define _ANDROS_MEMORY_TOP_LEVEL_HEADER_
#include "stddef.h"

void* malloc(size_t size);

void operator delete(void* target);
void operator delete[](void* target);

void* operator new(size_t size);
void* operator new[](size_t size);


#endif
