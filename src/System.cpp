#include "System.hpp"
#include <stdint.h>
#include "boot.hpp"

System::System()
  : mbhi(multiboot_header_ptr)
{
}

void System::init()
{
  initConsole();
  initMemoryManager();
  initInterrupts();
}

void System::initConsole()
{
  screen = &vgaTextConsole;
  screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  screen->clear();
  screen->print("Welcome to AndrOS 0.0.1b\n\0");
}


void System::initMemoryManager()
{
  kernelStackEnd = stack_ptr;
  kernelStackBegin = (void*)((uint32_t)(stack_ptr) - 16384);

  findBiggestChunkOfMemory();

  memoryManager.initGDT();
  
  //TODO: Finish
}

void System::initInterrupts()
{
  //TODO
}
