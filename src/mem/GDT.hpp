#ifndef _ANDROS_GDT_HPP_
#define _ANDROS_GDT_HPP_

#include "stdint.h"

// MACROS
#define GDT_ACCESS_PRESENT         = 1 << 7;
#define GDT_ACCESS_PRIVLEGE_RING_0 = 0x0 << 5; // Ring 0 = kernel. Ring 3 = userspace
#define GDT_ACCESS_PRIVLEGE_RING_1 = 0x1 << 5;
#define GDT_ACCESS_PRIVLEGE_RING_2 = 0x2 << 5;
#define GDT_ACCESS_PRIVLEGE_RING_3 = 0x3 << 5;
#define GDT_ACCESS_CODE_SEGMENT    = 1 << 4;
#define GDT_ACCESS_DATA_SEGMENT    = 0 << 4;
#define GDT_ACCESS_EXECUTABLE      = 1 << 3; // If this is a code selector. Always 0 for data segments
#define GDT_ACCESS_NOT_EXECUTABLE  = 0 << 3;
#define GDT_ACCESS_DIRECTION_UP    = 0 << 2; // For data selectors, if up than the segment grows up.
#define GDT_ACCESS_DIRECTION_DOWN  = 1 << 2; // Otherwise, it grows down (offset is higher than limit).
#define GDT_ACCESS_CONFORMING      = 1 << 2; // For code selectors, lower-ring, _conforming_ code can be 
                                             // called from a higher-ring segment.
#define GDT_ACCESS_NONCONFORMING   = 0 << 2; // If this bit is not set, this code can only be called from its own ring.
#define GDT_ACCESS_READABLE_CODE   = 1 << 1;
#define GDT_ACCESS_WRITABLE_DATA   = 1 << 1;
#define GDT_ACCESS_ACCESSED        = 1; // Just set to zero initially.

#define GDT_FLAGS_PAGE_GRANULARITY = 1 << 7; // If limit is in 4 KiB blocks
#define GDT_FLAGS_BYTE_GRANULARITY = 0 << 7;
#define GDT_FLAGS_SIZE_16_BIT_PROTECTED_MODE = 0 << 6;
#define GDT_FLAGS_SIZE_32_BIT_PROTECTED_MODE = 1 << 6;


struct GDTEntry
{
  uint32_t base;
  uint32_t limit; //Actually a 20-bit value
  uint8_t flags;
  uint8_t access;

  void encodeInMemory(uint8_t* destination);
  void decodeFromMemory(uint8_t* source);
};

#endif
