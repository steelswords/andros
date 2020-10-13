#include "MemoryManager.hpp"
#include "stdint.h"
#include "stddef.h"

void* malloc(size_t size)
{
  return MemoryManager::getSingleton()->allocate(size);
}
