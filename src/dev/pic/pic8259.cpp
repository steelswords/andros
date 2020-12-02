#include "dev/cpuIO.hpp"
#include "dev/pic/pic8259.hpp"
#include "stdint.h"

void sendEndOfInterrupt8259(uint8_t irq)
{
  if (irq >= 8)
  {
    //Then send to PIC2
    outb(PIC2_COMMAND_PORT, PIC_EOI_COMMAND);
  }
  else
  {
    // Then send to PIC1 and PIC2
    outb(PIC1_COMMAND_PORT, PIC_EOI_COMMAND);
    //outb(PIC2_COMMAND_PORT, PIC_EOI_COMMAND);
  }
}


/********************************************************
 * PIC INITIALIZATION CODE
 *
 * SOURCE: OSDEV WIKI
 *******************************************************/


/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void remap8259PIC(int offset1, int offset2)
{
	uint8_t mask1, mask2;
 
	mask1 = inb(PIC1_DATA_PORT);                        // save masks
	mask2 = inb(PIC2_DATA_PORT);
 
	outb(PIC1_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	ioWait();
	outb(PIC2_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);
	ioWait();
	outb(PIC1_DATA_PORT, offset1);                 // ICW2: Master PIC vector offset
	ioWait();
	outb(PIC2_DATA_PORT, offset2);                 // ICW2: Slave PIC vector offset
	ioWait();
	outb(PIC1_DATA_PORT, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	ioWait();
	outb(PIC2_DATA_PORT, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	ioWait();
 
	outb(PIC1_DATA_PORT, ICW4_8086);
	ioWait();
	outb(PIC2_DATA_PORT, ICW4_8086);
	ioWait();
 
	outb(PIC1_DATA_PORT, mask1);   // restore saved masks.
	outb(PIC2_DATA_PORT, mask2);
}

/* Disables a particular IRQ
 * This is accomplished by masking (setting to 1) the corresponding
 * bit in the PIC's interrupt mask register.
 *
 * From the OSDev Wiki
 */
void disableIRQ8259(uint8_t irqToDisable)
{
  uint16_t port;
  uint8_t value;
  if (irqToDisable < 8)
  {
    port = PIC1_DATA_PORT;
  }
  else
  {
    port = PIC2_DATA_PORT;
    irqToDisable -= 8;
  }

  value = inb(port) | (1 << irqToDisable);
  outb(port, value);
}

/* Enables a particular IRQ
 * This is accomplished by clearing the corresponding bit in the
 * PIC's interrupt mask register.
 *
 * From the OSDev Wiki
 */
void enableIRQ8259(uint8_t irqToEnable)
{
  uint16_t port;
  uint8_t value;
  if (irqToEnable < 8)
  {
    port = PIC1_DATA_PORT;
  }
  else
  {
    port = PIC2_DATA_PORT;
    irqToEnable -= 8;
  }

  value = inb(port) & ~(1 << irqToEnable);
  outb(port, value);
}
