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
extern "C" void* stack_ptr;

extern "C" char* vendorStringPtr;

extern "C" {
  int byteswap32(uint32_t value);
}

void kprint_greeting()
{
  VGATextConsoleScreen textConsole;
  screen = &textConsole;
  screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  screen->clear();
  char msg1[] = "Welcome to AndrOS 0.0.1b\n\0";
  screen->print(msg1);

#if 0
  uint32_t num32bit = 0x13579B;
  kstring msg32(num32bit, 16);
  screen->print(msg32);
  screen->putChar('\n');

  uint64_t num64bit = 0x1234567890abcdef;
  kstring msg64(num64bit);
  screen->print(msg64);

  screen->print("\nPrinted using printlHex(): ");
  screen->printlHex(num64bit);
#endif

#if 0

  screen->print("Kstring memory area begin: ");
  screen->printlHex((uint64_t)kstring_area_begin_ptr);
  screen->print("\nKstring memory area end: ");
  screen->printlHex((uint64_t)kstring_area_end_ptr);

  screen->print("\nMultiboot header: ");
  screen->printlHex((uint64_t)multiboot_header_ptr);
  screen->print("\nStack begin: ");
  screen->printlHex((uint64_t)stack_ptr);
  screen->printHex((uint32_t)0xFEEDC0DE);

  screen->print("Soon you will see the address of m_data of a kstring.\n");
  kstring kmsg("\nI can print kstrings!\nAddress of this kstring: ");
  screen->printlHex((uint64_t)kmsg.m_data);
  screen->print("Soon you will see the address of m_data of another kstring.\n");
  kstring kmsg3("Five");
  screen->printlHex((uint64_t)kmsg3.m_data);
  kstring kmsg2("\nHlp pls");
  kstring kmsg4("\nOk thx bye\n");
  screen->print(kmsg2);
  screen->print(kmsg4);
  screen->print("\nDebug Val: ");
  screen->print(kstring::debugVal);
#endif
  
  // Print location of multiboot header
  MultibootHeaderInfo mbh(multiboot_header_ptr);
#if 0
  screen->print("\nMultiboot header flags: ");
  uint32_t flags = *(uint32_t*)multiboot_header_ptr;
  screen->printHex(flags);
  screen->putChar('\n');
#endif
  screen->print("Upper memory available: ");
  screen->print(mbh.m_upperMemoryAvailable);
  screen->print(" KB\n");

  //screen->print("Hexdump of Multiboot header:\n");
  //memdump(screen, multiboot_header_ptr, 115); 

  mbh.printMemoryTable(screen);

  CPUIDInformation  cpuidinfo;
  screen->print("cpuinfo:");
  screen->printHex((uint32_t) &cpuidinfo );

  int r = identify_cpu(&cpuidinfo);
  screen->print("CPU Vendor @");
  screen->printHex((uint32_t)vendorStringPtr);
  screen->print(":");
  screen->print(cpuidinfo.vendorString);
  screen->nl();
  screen->print((char*)vendorStringPtr);

  screen->nl();
  screen->print("Returned ");
  screen->printHex(r);
  screen->nl();
#if 0
  screen->print("0x12345678 swapped is ");
  uint32_t swapped = byteswap32(0x12345678);
  screen->printHex(swapped);
#endif

  return;
}
