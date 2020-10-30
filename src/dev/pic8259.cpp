#include "pic8259.hpp"
#include "dev/cpuio.hpp"

void PICSendEOI(uint8_t irq)
{
  // IF the IRQ >= 8, send EOI to PIC2 instead of PIC1.
  if(irq >= 8)
  {
    outb(PIC2_COMMAND, PIC_EOI);
  }  
  else
  {
    outb(PIC1_COMMAND, PIC_EOI);
  }
}
