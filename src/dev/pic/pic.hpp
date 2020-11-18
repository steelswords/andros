#ifndef _ANDROS_PIC_GENERIC_HPP_
#define _ANDROS_PIC_GENERIC_HPP_

/**************************************************************
 * Name:            pic.hpp
 * Author:          Tristan Andrus
 * Description:     Provides a generic interface to the PIC, to allow a swapperoo
 *                  with an APIC implementation in the future.
 **************************************************************/

#include "dev/pic/pic8259.hpp"

/* Remaps IRQ 0 from PIC1 to Interrupt vector offset1
 * and likewise remaps the IRQs from PIC2 to start at offset2.
 */
inline void remapPIC(int offset1, int offset2)
{
  remap8259PIC(offset1, offset2);
}

inline void sendEndOfInterrupt(uint8_t irq)
{
  sendEndOfInterrupt8259(irq);
}

void enableIRQ(uint8_t irqToEnable)
{
  enableIRQ8259(irqToEnable);
}

void disableIRQ(uint8_t irqToDisable)
{
  disableIRQ8259(irqToDisable);
}

#endif
