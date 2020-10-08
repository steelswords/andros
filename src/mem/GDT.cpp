#include "mem/GDT.hpp"

#include "stdint.h"

// This code is modified from OSDev's Wiki: https://wiki.osdev.org/GDT_Tutorial
void GDTEntry::encodeInMemory(uint8_t* destination)
{
  if ((limit > 65536) && ((limit & 0xFFFF) != 0xFFFF))
  {
    //TODO: Raise error.
  }
  if (limit > 65536)
  {
    limit = limit >> 12;
    destination[6] = 0xC0; //TODO: Why?
  }
  else
  {
    destination[6] = 0x40; //TODO: Why?
  }

  // Encode the limit
  destination[0] = limit & 0xFF;
  destination[1] = (limit >> 8) & 0xFF;
  destination[6] |= (limit >> 16) & 0xF;

  // Encode the base
  destination[2] = base & 0xFF;
  destination[3] = (base >> 8)  & 0xFF;
  destination[4] = (base >> 16) & 0xFF;
  destination[7] = (base >> 24) & 0xFF;

  //Encode Flags
  destination[6] |= (flags & 0xF0);

  //Encode access
  destination[5] = access;

}

void GDTEntry::decodeFromMemory(uint8_t* source)
{
  // Assemble limit
  limit = source[0] | (source[1] << 8) | ((source[6] & 0xF) << 16);

  // Assemble base
  base = source[2] | (source[3] << 8) | (source[4] << 16) | (source[7] << 24);

  // Get Flags
  flags = source[6] & 0xF0;

  // Get access
  access = source[5];
  
}
