#ifndef _ANDROS_MULTIBOOT_HEADER_INFO_H
#define _ANDROS_MULTIBOOT_HEADER_INFO_H

#include <stdint.h>

#include "ConsoleScreen.hpp"

struct MBHIMemoryMapEntry
{
  uint32_t size;
  uint32_t baseAddressLow; //Sometimes GCC packs uint64_ts weird.
  uint32_t baseAddressHigh;
  uint32_t lengthLow;
  uint32_t lengthHigh;
  uint32_t type;
} __attribute__((packed));

class MultibootHeaderInfo
{
public:
   MultibootHeaderInfo(void* multibootHeaderLocation); 
   uint32_t m_lowerMemoryAvailable;
   uint32_t m_upperMemoryAvailable;
   char* m_bootCommand;
   char* m_moduleString;
   void* m_moduleStart;
   void* m_moduleEnd;
   MBHIMemoryMapEntry* m_mbhiMemoryMap;
   uint32_t m_mbhiMemoryMapLength;
   
   void printMemoryTable(ConsoleScreen* screen);
  

};

#endif
