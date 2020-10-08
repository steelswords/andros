#ifndef _ANDROS_IDT_HPP_
#define _ANDROS_IDT_HPP_

#include "stdint.h"

struct IDTDescriptor32Bit
{
  uint16_t offset1; //Offset bits 0-15
  uint16_t segementSelector; //Code segment selector in GDT or LDT
  uint8_t  zero; //Unused, set to 0
  uint8_t typeAttributes; //Type and attributes
  uint16_t offset2; //Offset bits 16-31
};

#endif
