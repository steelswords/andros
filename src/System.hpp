#ifndef _ANDROS_SYSTEM_HPP_
#define _ANDROS_SYSTEM_HPP_

#include "dev/hwid/MultibootHeaderInfo.hpp"
#include "video/VGATextConsoleScreen.hpp"
#include "mem/MemoryManager.hpp"

class System
{
public:
  MemoryManager memoryManager;
  MultibootHeaderInfo mbhi;
  ConsoleScreen* screen;

  /* Constructors */
  System();
  void init();

private: // Only called during construction.

  /* Fills the heap/stack begin/end pointers and
   * initiailizes the memory manager. */
  void initMemoryManager();
  void initConsole();
  void initInterrupts();

protected:
  void* kernelHeapBegin;
  void* kernelHeapEnd;
  void* kernelStackBegin;
  void* kernelStackEnd;

  /* Data */
  VGATextConsoleScreen vgaTextConsole;
};

#endif
