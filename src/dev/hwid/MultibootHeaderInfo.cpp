#include "MultibootHeaderInfo.hpp"
#include <stdint.h>

MultibootHeaderInfo::MultibootHeaderInfo(void* multibootHeaderLocation)
{
  uint32_t* header = (uint32_t*)multibootHeaderLocation;

  uint32_t flags = *header;
  
  //If the 0th bit of flags is set, then the memory fields are valid
  if ( (flags & 0x1) == 1)
  {
    header++;
    m_lowerMemoryAvailable = *header;
    header++;
    m_upperMemoryAvailable = *header;
  }
  else
  {
    header += 2;
  }

  // If the 1st bit of flags is set, then we can get information about
  // the boot device. I don't think we care about this right now.
  if ( (flags & 0x4) == 1)
  { 
    header++;
  }
  else
  {
  }

  //If bit 2 of flags is set, then command line is valid
  if ( (flags & 0x4) == 1)
  {
    m_bootCommand = (char*)*header;
    header++;
  }


  // If bit 3 of flags is set, then the mods field indicates 
  if ( (flags & 0x8) == 1)
  {
    header += 2;
  }

  // Bits 4 and 5 are for the kernel symbol names.
  // Don't need them right now
  if ( (flags & 0x10) || (flags & 0x20))
  {
    header += 4;
  }

  //Bit 6: BIOS memory maps
  if ( (flags & 0x40) == 1)
  {
    m_mbhiMemoryMapLength = *header;
    header++;
    m_mbhiMemoryMap = (MBHIMemoryMapEntry*)header;
    header++;
  }

  //Bit 7: Drive table
  

}

void MultibootHeaderInfo::printMemoryTable(ConsoleScreen* screen)
{
  MBHIMemoryMapEntry* mmap = m_mbhiMemoryMap;
  //Iterate through the entire buffer
  //while(entry < m_mbhiMemoryMap + m_mbhiMemoryMapLength)
  {
    screen->print("Entry at ");
    screen->printHex((int)mmap);
  }

}
