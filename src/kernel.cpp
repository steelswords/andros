/* vga_text_kernel.c
 * A kernel, taken straight from the OSDev.org Bare Bones tutorial.
 * This uses VGA text mode, which is apparently deprecated on modern
 * hardware. A lovely starting point. Well, on to a graphics-based 
 * interface. */
#include <stdbool.h>
//#include <stddef.h>
#include <stdint.h>
#include "video/VGATextConsoleScreen.hpp"
#include "video/ConsoleScreen.hpp"
//#include "libc/include/stdlib.h"
#include "utils/kstring.hpp"
#include "utils/utils.hpp"
#include "dev/hwid/MultibootHeaderInfo.hpp"
#include "dev/hwid/CPUID.hpp"
#include "mem/MemoryManager.hpp"
#include "System.hpp"
#include "boot.hpp"
#include "mem/memory.hpp"
#include "dev/cpuIO.hpp"
#include "KernelTerminal.hpp"
#include "dev/keyboardScancodes.hpp"
#include "utils/CircularBuffer.hpp"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This kernel will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/* This loop is never used, but is needed to compile without
 * optimizations. */
extern "C" void __cxa_pure_virtual() { while (1); }

/* Global objects */
ConsoleScreen* screen;
System* system;
KernelTerminal* kterm = nullptr;

//TODO: Get rid of this
void kprint_greeting();

//TODO: Get rid of this
struct testStruct
{
  uint32_t a;
  uint32_t b;
};
 
extern "C" 
{
void kernel_main(void) 
{
  System sys;
  system = &sys;
  sys.init();
  screen = sys.screen;

  // Initialize kterm
  KernelTerminal _kernelTerminal;
  kterm = &_kernelTerminal;
  kterm->m_stdout = screen;
  screen->print("Kernel Terminal Initialized\n");

  screen->print("\nAndrOS Initialized\n");

#if 1 
  inb(0x60);
  uint32_t oldKey = 0;
  while (1)
  {
    uint32_t key = inb(0x60);
    if (key != oldKey)
    {
#if 0
      //Prints scancode
      screen->print(" <");
      screen->print(key);
      screen->print(">");
#endif
      system->keystrokes->add((char)key);
      processKeyboardInput(system->keystrokes, kterm->m_stdin);
      kterm->handleInput();
      oldKey = key;
    }
  }
#else
  while (1) ;
#endif

}
}

extern "C" {
  int increment(int i);
}

void kprint_greeting()
{
#if 0
  VGATextConsoleScreen textConsole;
  screen = &textConsole;
  screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  screen->clear();
  char msg1[] = "Welcome to AndrOS 0.0.1b\n\0";
  screen->print(msg1);
#endif

  // Print location of multiboot header
  MultibootHeaderInfo mbh(multiboot_header_ptr);

  screen->print("Upper memory available: ");
  screen->print(mbh.m_upperMemoryAvailable);
  screen->print(" KB\n");

  mbh.printMemoryTable(screen);

  screen->print("Initializing GDT...");
  MemoryManager mman;
  mman.initGDT();

  memdump(screen, &mman.GDTBuffer[0], mman.m_numberGDTEntries * GDT_ENTRY_SIZE_IN_MEMORY);

  screen->nl();
  screen->printHex((uint32_t)mman.GDTBuffer);
  screen->print("\nSize:");
  screen->print(mman.m_numberGDTEntries * GDT_ENTRY_SIZE_IN_MEMORY);

  screen->print("Done.");

  return;
}
