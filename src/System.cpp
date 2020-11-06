#include "System.hpp"
#include "dev/hwid/MultibootHeaderInfo.hpp"
#include <stdint.h>
#include "boot.hpp"
#include "memory.hpp"

System::System()
  : mbhi(multiboot_header_ptr)
{
}

void System::init()
{
  initConsole();
  //initMemoryManager();
  //cpuInfo = new CPUInformation();
  //initInterrupts();
}

void System::initConsole()
{
  screen = &vgaTextConsole;
  screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  screen->clear();
  screen->print("Welcome to AndrOS 0.0.1c\n\0");
  screen->print("Welcome to AndrOS 0.0.1c\n\0");
  screen->print("Is there a problem here?\n");
  screen->print("No, seriously. Is there?\n");
  screen->print("Cause I kinda need to know.\n");
  screen->print("Can I at least print a number? ");
  screen->print(33);
  screen->print(" Maybe...");
}

void System::findBiggestChunkOfMemory(void*& begin, void*& end)
{
  MBHIMemoryMapEntry* entry = mbhi.m_mbhiMemoryMap;
  uint64_t contenderBegin = 56;
  uint64_t contenderEnd = 0;

  //Iterate through the entire buffer
  while((uint64_t)entry < ((uint64_t)mbhi.m_mbhiMemoryMap + (uint64_t)mbhi.m_mbhiMemoryMapLength))
  {
    // Computations
    uint64_t baseAddress = (((uint64_t)entry->baseAddressHigh) << 32) + ((uint64_t)entry->baseAddressLow);
    uint64_t endAddress  = ((uint64_t)entry->baseAddressHigh << 32) + ((uint64_t)entry->baseAddressLow);
    uint64_t length      = ((uint64_t)entry->lengthHigh << 32) + ((uint64_t)entry->lengthLow);
    endAddress += length;

    if (entry->type == Available)
    {
      //if (length > (contenderEnd - contenderBegin))
      {
        contenderEnd = endAddress;
        contenderBegin = baseAddress;
      }
    }
    
    // Update entry
    uint64_t buf = (uint64_t)entry;
    buf += entry->size + sizeof(entry->size);
    entry = (MBHIMemoryMapEntry*)buf;
  }

  begin = (void*)contenderBegin;
  end   = (void*)contenderEnd;
}

//#define DEBUG_PRINTOUTS
void System::initMemoryManager()
{
  kernelStackEnd = stack_ptr;
  kernelStackBegin = (void*)((uint32_t)(stack_ptr) - 16384);

  findBiggestChunkOfMemory(kernelHeapBegin, kernelHeapEnd);
#ifdef DEBUG_PRINTOUTS
  screen->print("Printing MBH info:\n");
  mbhi.printMemoryTable(screen);
  screen->nl();
  screen->print("Largest chunk of memory at: ");
  screen->printlHex((uint64_t)kernelHeapBegin);
  screen->print(" to ");
  screen->printlHex((uint64_t)kernelHeapEnd);
  screen->nl();
#endif

  
  memoryManager.initGDT();

  memoryManager.m_heapStart = kernelHeapBegin;
  memoryManager.m_heapEnd   = kernelHeapEnd;
  memoryManager.m_heapCur   = kernelHeapBegin;

#ifdef DEBUG_PRINTOUTS
  screen->print("Size of a size_t:");
  screen->print(sizeof(size_t));
  screen->nl();

  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printlHex((uint64_t)memoryManager.m_heapCur);
  screen->nl();
#endif
  uint32_t* pInt1 = (uint32_t*)malloc(sizeof(uint32_t));
#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a 4 byte structure.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
#endif

  uint32_t* pInt2 = (uint32_t*)malloc(sizeof(uint32_t));

  if (pInt1 == pInt2)
  {
    //TODO: Throw error.
    screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_RED);
    screen->print("ERROR! Allocator overlapping memory!");
    screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  }

#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a 4 byte structure.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
  screen->print("Allocated two 32-bit pointers: ");
  screen->printHex((uint32_t)pInt1);
  screen->print(" and ");
  screen->printHex((uint32_t)pInt2);
  screen->nl();
#endif

  //Test new operator
  GDTEntry* testEntry = new GDTEntry();
#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a ");
  screen->printHex(sizeof(GDTEntry));
  screen->print(" byte structure with new operator.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
#endif

  GDTEntry* testEntry2 = new GDTEntry();
#ifdef DEBUG_PRINTOUTS
  screen->print("[DEBUG] Allocated a pointer to a ");
  screen->printHex(sizeof(GDTEntry));
  screen->print(" byte structure with new operator.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)memoryManager.m_heapCur);
  screen->nl();
#endif
  
}

#if 0
__attribute__((interrupt))
void handler(InterruptFrame* frame)
#endif

void System::initInterrupts()
{
  //Initialize IDT (Already done in the constructor for IDT)
  idt.writeToMemory();

  screen->print("\ninterruptReturnCode: ");
  screen->print(IDT::interruptReturnCode);
  screen->print(". Now testing ISR...\n");

  //Check that interruptReturnCode is 32 to begin with. 
  testInterrupts();

  screen->print("interruptReturnCode: ");
  screen->print(IDT::interruptReturnCode);

  screen->print("Done.");
    
}
