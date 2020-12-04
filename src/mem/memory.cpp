#include "memory.hpp"
#include "MemoryManager.hpp"
#include "stdint.h"
#include "stddef.h"

void* malloc(size_t size)
{
  return MemoryManager::getSingleton()->allocate(size);
}

#if 0
// Dummy delete operator so I can have objects.
// G++ generates a call to this operator when compiling the destructors.
// Eventually this will be implemented fully.
void operator delete(void * target)
{
}

void operator delete[](void * target)
{
}

void* operator new(size_t size)
{
  return malloc(size);
}

void* operator new[](size_t size)
{
  return malloc(size);
}
#endif
