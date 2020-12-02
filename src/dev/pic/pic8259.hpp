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

void sendEndOfInterrupt8259(uint8_t irq);


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
void remap8259PIC(int offset1, int offset2);

/* Disables a particular IRQ
 * This is accomplished by masking (setting to 1) the corresponding
 * bit in the PIC's interrupt mask register.
 *
 * From the OSDev Wiki
 */
void disableIRQ8259(uint8_t irqToDisable);

/* Enables a particular IRQ
 * This is accomplished by clearing the corresponding bit in the
 * PIC's interrupt mask register.
 *
 * From the OSDev Wiki
 */
void enableIRQ8259(uint8_t irqToEnable);

#endif
