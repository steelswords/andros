#include "mem/MemoryManager.hpp"
#include "stdint.h"


MemoryManager::MemoryManager()
{
}

void MemoryManager::initGDT(void* location, uint16_t size)
{
  // TODO: A more robust memory management system
  
  // Manually create some GDT entries
  GDTEntries[0].type = 

  // load them into memory
  loadGDTEntries();

  // call assembly routine to load GDT again.
  _loadGDT(GDTBuffer, 5*GDT_ENTRY_SIZE_IN_MEMORY); // Replace 5 with however many manual descriptors we make. 
  
}

void MemoryManager::loadGDTEntries()
{
  int offset = 0;
  for(int i = 0; i < m_maximumGDTEntries; ++i)
  {
    offset = i * GDT_ENTRY_SIZE_IN_MEMORY; // 8 bytes/GDTEntry in memory
    GDTEntries[i].encodeInMemory(&GDTBuffer[offset]);
  }
}
