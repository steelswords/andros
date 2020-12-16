#include "KernelTerminal.hpp"
#include "utils/kstring.hpp"
#include "System.hpp"
#include "stddef.h"

void KernelTerminal::peek(uint32_t address)
{
  // Get integer for first argument
  // Read value and
  // Display
  m_stdout->print("Memory location at ");
  m_stdout->printHex(address);
  m_stdout->print(": ");
  uint8_t value = *(uint8_t*)address;
  m_stdout->printHex(value);
  m_stdout->nl();
}

void KernelTerminal::poke(uint8_t* address, uint8_t value)
{
  m_stdout->print("Changing location ");
  m_stdout->printHex((uint32_t)address);
  m_stdout->print(" to ");
  m_stdout->printHex(value);
  m_stdout->nl();
  *address = value;
}

void KernelTerminal::printOSName()
{
  m_stdout->print("AndrOS v0.0.1\n");
}

void KernelTerminal::cpuid()
{
  System::getInstance()->cpuInfo->print(m_stdout);
  m_stdout->nl();
}

void KernelTerminal::printMemoryMap()
{
  System::getInstance()->mbhi.printMemoryTable(m_stdout);
  m_stdout->nl();
}

void KernelTerminal::demoMalloc(uint32_t sizeToAllocate)
{
  System* sys = System::getInstance();
  ConsoleScreen* screen = m_stdout;

  screen->print("Size of a size_t:");
  screen->print(sizeof(size_t));
  screen->nl();

  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printlHex((uint64_t)sys->memoryManager.m_heapCur);
  screen->nl();
  uint32_t* pInt1 = (uint32_t*)malloc(sizeof(uint32_t));
  screen->print("[DEBUG] Allocated a pointer to a 4 byte structure.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)sys->memoryManager.m_heapCur);
  screen->nl();

  uint32_t* pInt2 = (uint32_t*)malloc(sizeof(uint32_t));

  if (pInt1 == pInt2)
  {
    //TODO: Throw error.
    screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_RED);
    screen->print("ERROR! Allocator overlapping memory!");
    screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  }

  screen->print("[DEBUG] Allocated a pointer to a 4 byte structure.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)sys->memoryManager.m_heapCur);
  screen->nl();
  screen->print("Allocated two 32-bit pointers: ");
  screen->printHex((uint32_t)pInt1);
  screen->print(" and ");
  screen->printHex((uint32_t)pInt2);
  screen->nl();

#if 0
  //Test new operator
  GDTEntry* testEntry = new GDTEntry();
  screen->print("[DEBUG] Allocated a pointer to a ");
  screen->print(sizeof(GDTEntry));
  screen->print(" byte structure with new operator.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)sys->memoryManager.m_heapCur);
  screen->nl();

  GDTEntry* testEntry2 = new GDTEntry();
  screen->print("[DEBUG] Allocated a pointer to a ");
  screen->print(sizeof(GDTEntry));
  screen->print(" byte structure with new operator.\n");
  screen->print("[DEBUG] Allocator currently points to: ");
  screen->printHex((uint32_t)sys-x>memoryManager.m_heapCur);
  screen->nl();
#endif
}

void KernelTerminal::printHelp()
{
  char helpText[] = 
"Valid built-in commands:\n\
 greet             Print a friendly message\n\
 clear             Clear the screen\n\
 osname            Prints the OS name & version\n\
 peek <addr>       Prints the value @ <addr>\n\
 poke <addr> <val> Puts <val> in memory @ <addr>\n\
 cpuid             Prints information about the CPU\n\
 malloc            Demonstrates memory allocation\n\
 memmap            Prints a map of memory locations\n\
";
  m_stdout->print(helpText);
}
