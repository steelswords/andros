#ifndef _ANDROS_8259_PIC_HPP
#define _ANDROS_8259_PIC_HPP
#include "stdint.h"


// First, some macros/addresses to make life more readable.
#define PIC1          0x20
#define PIC2          0xA0
#define PIC1_COMMAND  (PIC1)
#define PIC1_DATA     (PIC1 + 1)
#define PIC2_COMMAND  PIC2
#define PIC2_DATA     (PIC2 + 1)

#define PIC_EOI 0x20 // End of interrupt code

void PICSendEOI(uint8_t irq);



#endif
