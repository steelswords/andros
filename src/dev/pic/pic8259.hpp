#ifndef _ANDROS_PIC_8259_HPP_
#define _ANDROS_PIC_8259_HPP_

#include "dev/cpuIO.hpp"
#include "stdint.h"

#define PIC1_PORT 0x20
#define PIC2_PORT 0xA0
#define PIC1_COMMAND_PORT PIC1_PORT
#define PIC1_DATA_PORT (PIC1_PORT + 1)
#define PIC2_COMMAND_PORT PIC2_PORT
#define PIC2_DATA_PORT (PIC2_PORT + 1)

// Some PIC commands
#define PIC_EOI_COMMAND 0x20



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
    outb(PIC2_COMMAND_PORT, PIC_EOI_COMMAND);
  }
}


/********************************************************
 * PIC INITIALIZATION CODE
 *
 * SOURCE: OSDEV WIKI
 *******************************************************/

// PIC Initialization commands
// ICW = Initialization Command Word
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */
 
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

#endif
