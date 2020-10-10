#ifndef _ANDROS_MEMORY_MANAGER_HPP
#define _ANDROS_MEMORY_MANAGER_HPP

#include "stdint.h"
#include "mem/GDT.hpp"

//TODO: Make a vector/linked list class
//TODO: Put GDTs in there

#define GDT_ENTRY_SIZE_IN_MEMORY 8
#define NUM_GDT_ENTRIES 16
#define GDT_MEMORY_SIZE_BYTES (GDT_ENTRY_SIZE_IN_MEMORY * NUM_GDT_ENTRIES)

class MemoryManager
{
public:
  MemoryManager();
  void initGDT();
  int m_numberGDTEntries {NUM_GDT_ENTRIES};
  GDTEntry GDTEntries[NUM_GDT_ENTRIES];
  uint8_t GDTBuffer[GDT_MEMORY_SIZE_BYTES] {0};
  
  /* Loads the GDTEntries from GDTEntries to gdtBuffer */
  void loadGDTEntries(); 
protected:
};


extern "C" void _loadGDT(void* GDTLocation, uint16_t size);

#endif
