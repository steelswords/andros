#ifndef _ANDROS_MEMORY_MANAGER_HPP
#define _ANDROS_MEMORY_MANAGER_HPP

#include "stdint.h"
#include "stddef.h"
#include "mem/GDT.hpp"

//TODO: Make a vector/linked list class
//TODO: Put GDTs in there

#define GDT_ENTRY_SIZE_IN_MEMORY 8
#define NUM_GDT_ENTRIES 16
#define GDT_MEMORY_SIZE_BYTES (GDT_ENTRY_SIZE_IN_MEMORY * NUM_GDT_ENTRIES)

class MemoryManager
{
protected:
  static MemoryManager* m_singleton;
  void* m_heapStart;
  void* m_heapEnd;
  void* m_heapCur;
  uint32_t m_dummy;
  friend class System;


public:
  MemoryManager();
  void initGDT();
  int m_numberGDTEntries {NUM_GDT_ENTRIES};
  GDTEntry GDTEntries[NUM_GDT_ENTRIES];
  uint8_t GDTBuffer[GDT_MEMORY_SIZE_BYTES] {0};
  
  /* Loads the GDTEntries from GDTEntries to gdtBuffer */
  void loadGDTEntries(); 

  static MemoryManager* getSingleton() { return m_singleton; }

  /* Allocates a chunk of memory */
  void* allocate(size_t size);
};

struct GDTPointer
{
  uint16_t size;
  uint32_t address;
} __attribute__((packed));

extern "C" void _loadGDT(GDTPointer* gdtpointer);

#endif
