#ifndef _ANDROS_CPU_IO_HPP
#define _ANDROS_CPU_IO_HPP

#include "stdint.h"

/* Helper functions for IO and memory access */

/* Writes data to an I/O port.
 * From the OSDev Wiki
 */
static inline void outb(uint16_t port, uint8_t value)
{
  asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port) );
}


/* Returns data from an I/O port.
 * From the OSDev Wiki
 */
static inline uint8_t inb(uint16_t port)
{
  uint8_t returnValue;
  asm volatile ( "inb %1, %0"
      : "=a"(returnValue)
      : "Nd"(port) );
  return returnValue;
}

/* Makes the CPU wait until an I/O operation finishes.
 * From the OSDev Wiki
 */
static inline void ioWait()
{
  asm volatile ( "outb %%al, $0x80" : :  "a"(0) );
}
#endif
