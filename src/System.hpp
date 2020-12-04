#ifndef _ANDROS_SYSTEM_HPP_
#define _ANDROS_SYSTEM_HPP_

#include "dev/hwid/CPUID.hpp"
#include "dev/hwid/MultibootHeaderInfo.hpp"
#include "video/VGATextConsoleScreen.hpp"
#include "mem/MemoryManager.hpp"
#include "dev/interrupts.hpp"
#include "utils/CircularBuffer.hpp"
#include "KernelTerminal.hpp"

class System
{
public:
  MemoryManager memoryManager;
  MultibootHeaderInfo mbhi;
  ConsoleScreen* screen;
  IDT* idt;
  CircularBuffer<uint8_t> m_keystrokeBuffer;
  CircularBuffer<uint8_t>* keystrokes;
  KernelTerminal* terminal;
  KernelTerminal m_terminal;


  static System* getInstance();

  CPUInformation* cpuInfo;

  /* Constructors */
  System();
  void init();

  /* Polls the keyboard controller for input and handles it. */
  void pollKeyboardAndHandle();

private: // Only called during construction.

  /* Fills the heap/stack begin/end pointers and
   * initiailizes the memory manager. */
  void initMemoryManager();
  void initConsole();
  void initInterrupts();
  void initTerminal();
  void findBiggestChunkOfMemory(void*& begin, void*& end);

protected:
  void* kernelHeapBegin;
  void* kernelHeapEnd;
  void* kernelStackBegin;
  void* kernelStackEnd;

  /* Data */
  VGATextConsoleScreen vgaTextConsole;
  static System* instance;
};

#endif
