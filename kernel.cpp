/* vga_text_kernel.c
 * A kernel, taken straight from the OSDev.org Bare Bones tutorial.
 * This uses VGA text mode, which is apparently deprecated on modern
 * hardware. A lovely starting point. Well, on to a graphics-based 
 * interface. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "video/VGATextConsoleScreen.hpp"
#include "video/ConsoleScreen.hpp"
 
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

void kprint_greeting()
{
  VGATextConsoleScreen textConsole;
  screen = &textConsole;
  screen->setConsoleColors(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_WHITE);
  screen->clear();
  char* msg1 = "Welcome to AndrOS 0.0.1a\n";
  char* msg2 = "Newline successful!\n";
  screen->print(msg1);
  screen->print(msg2);

  return;
}
