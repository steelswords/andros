#ifndef _ANDROS_SYSTEM_HPP_
#define _ANDROS_SYSTEM_HPP_

#include "dev/hwid/CPUID.hpp"
#include "dev/hwid/MultibootHeaderInfo.hpp"
#include "video/VGATextConsoleScreen.hpp"
#include "mem/MemoryManager.hpp"
#include "dev/interrupts.hpp"

class System
{
public:
  MemoryManager memoryManager;
  MultibootHeaderInfo mbhi;
  ConsoleScreen* screen;
  IDT* idt;


  CPUInformation* cpuInfo;

  /* Constructors */
  System();
  void init();

private: // Only called during construction.

  /* Fills the heap/stack begin/end pointers and
   * initiailizes the memory manager. */
  void initMemoryManager();
  void initConsole();
  void initInterrupts();
  void findBiggestChunkOfMemory(void*& begin, void*& end);

protected:
  void* kernelHeapBegin;
  void* kernelHeapEnd;
  void* kernelStackBegin;
  void* kernelStackEnd;

  /* Data */
  VGATextConsoleScreen vgaTextConsole;
};

#endif
