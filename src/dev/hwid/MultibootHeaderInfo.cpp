#include "MultibootHeaderInfo.hpp"
#include "ConsoleScreen.hpp"
#include "utils/utils.hpp"
#include <stdint.h>

#define MBHI_MEM_FLAGS_INDEX        0
#define MBHI_MEM_LOWER_INDEX        1
#define MBHI_MEM_UPPER_INDEX        2
#define MBHI_BOOT_DEVICE_INDEX      3
#define MBHI_CMDLINE_INDEX          4
#define MBHI_MODS_COUNT_INDEX       5
#define MBHI_MODS_ADDR_INDEX        6
#define MBHI_SYMS_INDEX             7
#define MBHI_MMAP_LENGTH_INDEX      11
#define MBHI_MMAP_ADDR_INDEX        12
#define MBHI_DRIVES_LENGTH_INDEX    13
#define MBHI_DRIVES_ADDR_INDEX      14
#define MBHI_CONFIG_TABLE_INDEX     15
#define MBHI_BOOT_LOADER_NAME_INDEX 16
#define MBHI_APM_TABLE_INDEX        17


MultibootHeaderInfo::MultibootHeaderInfo(void* multibootHeaderLocation)
{
  uint32_t* header = (uint32_t*)multibootHeaderLocation;

  uint32_t flags = *header;
  
  //If the 0th bit of flags is set, then the memory fields are valid
  if (flags & 0x1)
  {
    m_lowerMemoryAvailable = header[MBHI_MEM_LOWER_INDEX];
    m_upperMemoryAvailable = header[MBHI_MEM_UPPER_INDEX];
  }
  

  // If the 1st bit of flags is set, then we can get information about
  // the boot device. I don't think we care about this right now.
  if (flags & 0x2)
  { 
  }

  //If bit 2 of flags is set, then command line is valid
  if (flags & 0x4)
  {
    m_bootCommand = (char*)header[MBHI_CMDLINE_INDEX];
  }


  // If bit 3 of flags is set, then the mods field indicates 
  if ( (flags & 0x8))
  {
  }

  // Bits 4 and 5 are for the kernel symbol names.
  // Don't need them right now
  if ( (flags & 0x10) || (flags & 0x20))
  {
  }

  //Bit 6: BIOS memory maps
  if ( (flags & 0x20))
  {
    m_mbhiMemoryMapLength = header[MBHI_MMAP_LENGTH_INDEX];
    m_mbhiMemoryMap = (MBHIMemoryMapEntry*)header[MBHI_MMAP_ADDR_INDEX];
  }

  //Bit 7: Drive table
  

}

void MultibootHeaderInfo::printMemoryTable(ConsoleScreen* screen)
{
  MBHIMemoryMapEntry* entry = m_mbhiMemoryMap;
  screen->print("MBHI @ ");
  screen->printlHex((uint64_t)this);
  screen->print("\nMemory Map @ ");
  screen->printlHex((uint64_t)m_mbhiMemoryMap);
  screen->print("   Length: ");
  screen->print(m_mbhiMemoryMapLength);
  screen->putChar('\n');

  //Iterate through the entire buffer
  while((uint64_t)entry < ((uint64_t)m_mbhiMemoryMap + (uint64_t)m_mbhiMemoryMapLength))
  {
    // Computations
    uint64_t baseAddress = (((uint64_t)entry->baseAddressHigh) << 32) + ((uint64_t)entry->baseAddressLow);
    uint64_t endAddress = ((uint64_t)entry->baseAddressHigh << 32) + ((uint64_t)entry->baseAddressLow);
    uint64_t length = ((uint64_t)entry->lengthHigh << 32) + ((uint64_t)entry->lengthLow);
    endAddress += length;

    // Print all the computations
    //screen->print("Entry at ");
    //screen->printlHex((uint64_t)entry);
    //screen->print(":\n");

    screen->printlHex(baseAddress);
    screen->print(" to ");
    screen->printlHex(endAddress);
#if 1
    screen->print(" (Size = ");
    screen->print(length >> 10);
    screen->print("KB). Type = ");
#else
    screen->print(". Type = ");
#endif

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
        screen->print("UNKNOWN (");
        screen->print(entry->type);
        screen->print(").");
    }
    //screen->print(" Size of entry: ");
    //screen->print(entry->size);
    screen->putChar('\n');
    
    // Update entry
    uint64_t buf = (uint64_t)entry;
    buf += entry->size + sizeof(entry->size);
    entry = (MBHIMemoryMapEntry*)buf;
  }

}
