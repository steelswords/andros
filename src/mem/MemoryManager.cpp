#include "mem/MemoryManager.hpp"
#include "mem/GDT.hpp"
#include "stdint.h"


MemoryManager::MemoryManager()
{
}

void MemoryManager::initGDT()
{
  // TODO: A more robust memory management system
  // Right now we're just selecting the whole 4 GiB space as code and data.
  
  // Manually create some GDT entries
  // First, the null selector
  GDTEntries[0].base = 0;
  GDTEntries[0].limit = 0;
  GDTEntries[0].flags = 0;
  GDTEntries[0].access = 0;

  //uint8_t defaultFlags = GDT_FLAGS_BYTE_GRANULARITY | GDT_FLAGS_SIZE_32_BIT_PROTECTED_MODE; 
  uint8_t defaultFlags = (0<<7) | GDT_FLAGS_SIZE_32_BIT_PROTECTED_MODE; 
  uint8_t defaultAccess = (uint8_t)(GDT_ACCESS_CODE_SEGMENT) | GDT_ACCESS_PRESENT;

  GDTEntries[1].base = 0;
  GDTEntries[1].limit = 0xffffffff;
  GDTEntries[1].flags = defaultFlags;
  GDTEntries[1].access = defaultAccess | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READABLE_CODE;

  //
  GDTEntries[2].base = 0;
  GDTEntries[2].limit = 0xffffffff;
  GDTEntries[2].flags = defaultFlags;
  GDTEntries[2].access = defaultAccess | GDT_ACCESS_NOT_EXECUTABLE | GDT_ACCESS_READABLE_CODE;

  m_numberGDTEntries = 3;

  // load them into memory
  loadGDTEntries();

  // call assembly routine to load GDT again.
  _loadGDT(&GDTBuffer[0], m_numberGDTEntries * GDT_ENTRY_SIZE_IN_MEMORY);
  
}

void MemoryManager::loadGDTEntries()
{
  int offset = 0;
  for(int i = 0; i < m_numberGDTEntries; ++i)
  {
    offset = i * GDT_ENTRY_SIZE_IN_MEMORY; // 8 bytes/GDTEntry in memory
    GDTEntries[i].encodeInMemory(&GDTBuffer[offset]);
  }
}
