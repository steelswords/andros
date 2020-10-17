#include "mem/MemoryManager.hpp"
#include "mem/GDT.hpp"
#include "stdint.h"

MemoryManager* MemoryManager::m_singleton = nullptr;

MemoryManager::MemoryManager()
  : m_heapStart(&m_dummy),
    m_heapEnd(&m_dummy),
    m_heapCur(&m_dummy)
{
  if (m_singleton == nullptr)
  {
    MemoryManager::m_singleton = this;
  }
  else
  {
    //TODO: Throw an error.
  }
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
#if 0
  //_loadGDT(&GDTBuffer[0], m_numberGDTEntries * GDT_ENTRY_SIZE_IN_MEMORY);
  GDTPointer gdt;
  gdt.size = m_numberGDTEntries * GDT_ENTRY_SIZE_IN_MEMORY;
  gdt.address = (uint32_t)GDTBuffer;
  _loadGDT(&gdt);
#endif
  
}

void MemoryManager::loadGDTEntries()
{
  int offset = 0;
  for(int i = 1; i < m_numberGDTEntries; ++i)
  {
    offset = i * GDT_ENTRY_SIZE_IN_MEMORY; // 8 bytes/GDTEntry in memory
    GDTEntries[i].encodeInMemory(&GDTBuffer[offset]);
  }
}

void* MemoryManager::allocate(size_t size)
{
  //This is currently a watermark allocater. No deallocation here.
  void* returnAddress = m_heapCur;
  m_heapCur = (void*)(((uint64_t)m_heapCur) + size);
  return returnAddress;
}
