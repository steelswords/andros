#include "MultibootHeaderInfo.hpp"
#include "ConsoleScreen.hpp"
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
  MBHIMemoryMapEntry* entry = m_mbhiMemoryMap;
  //Iterate through the entire buffer
  while(entry < m_mbhiMemoryMap + m_mbhiMemoryMapLength)
  {
    // Computations
    uint64_t baseAddress = ((uint64_t)entry->baseAddressHigh << 32) + (uint64_t)entry->baseAddressLow;
    uint64_t endAddress = ((uint64_t)entry->baseAddressHigh << 32) + entry->baseAddressLow;
    uint64_t length = ((uint64_t)entry->lengthHigh << 32) + entry->lengthLow;
    endAddress += length;

    // Print all the computations
    screen->print("Entry at ");
    screen->printlHex((uint64_t)entry);
    screen->print(":\n");

    screen->printlHex(baseAddress);
    screen->print(" to ");
    screen->printlHex(endAddress);
    screen->print(". Type = ");

    switch(entry->type)
    {
      case Available:
        screen->print("AVAILABLE");
        break;
      case ACPIInformation:
        screen->print("ACPI INFORMATION");
        break;
      case Reserved:
        screen->print("RESERVED");
        break;
      case DefectiveOrReservedArea:
        screen->print("DEFECTIVE OR RESERVED AREA");
        break;
      default:
        screen->print("UNKNOWN. TF, BRUH?");
    }
    screen->print("\n");
    
    
    entry += entry->size;
  }

}
