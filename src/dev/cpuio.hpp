#ifndef _ANDROS_CPU_IO_HPP_
#define _ANDROS_CPU_IO_HPP_
#include "stdint.h"

// Thanks to wiki.osdev.org for much of this code.

static inline void outb(uint16_t port, uint16_t data)
{
  asm volatile( "outb %0, %1" : : "a"(data), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
  uint8_t returnValue;
  asm volatile("inb %1, %0" : "=a"(returnValue) : "Nd"(port) );
  return returnValue;
}

static inline uint8_t inw(uint16_t port)
{
  uint8_t returnValue;
  asm volatile("inw %1, %0" : "=a"(returnValue) : "Nd"(port) );
  return returnValue;
}
static inline uint8_t inl(uint16_t port)
{
  uint8_t returnValue;
  asm volatile("inl %1, %0" : "=a"(returnValue) : "Nd"(port) );
  return returnValue;
}
#endif
