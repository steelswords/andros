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
#include "dev/hwid/MultibootHeaderInfo.hpp"
 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

// Dummy delete operator so I can have objects.
// G++ generates a call to this operator when compiling the destructors.
// Eventually this will be implemented fully.
void operator delete(void * target)
{
}

/* This loop is never used, but is needed to compile without
 * optimizations. */
extern "C" void __cxa_pure_virtual() { while (1); }

extern void* multiboot_header_ptr;

ConsoleScreen* screen;

void kprint_greeting();
 
extern "C" 
{
void kernel_main(void) 
{
  kprint_greeting();
  while (1) ;
}
}

extern "C" {
  int increment(int i);
}

void kprint_greeting()
{
  VGATextConsoleScreen textConsole;
  screen = &textConsole;
  screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  screen->clear();
  char msg1[] = "Welcome to AndrOS 0.0.1a\n\0";
  screen->print(msg1);

  screen->print("Kstring memory area begin: ");
  screen->printlHex((uint64_t)kstring_area_begin);
  screen->print("\nKstring memory area end: ");
  screen->printlHex((uint64_t)kstring_area_end);

  screen->print("\nMultiboot header: ");
  screen->printlHex((uint64_t)multiboot_header_ptr);
  return;
  screen->printHex((uint32_t)0xFEEDC0DE);

  kstring kmsg("\nAnd kstrings!\nAddress of this kstring: ");
  //screen->printlHex((uint64_t)kmsg.m_data);
  kstring kmsg2("\nHere's a second kstring\n");
  screen->print(kmsg);
  screen->print(kmsg2);
  screen->printlHex((uint64_t)kmsg2.m_data);
  

  // Print location of multiboot header
  MultibootHeaderInfo mbh(multiboot_header_ptr);
  screen->print("\nMultiboot header: ");
  uint64_t flags = *(uint64_t*)multiboot_header_ptr;
  screen->printlHex(flags);

  //mbh.printMemoryTable(screen);

  return;
}
