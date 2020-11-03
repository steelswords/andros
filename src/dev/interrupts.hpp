#ifndef _ANDROS_INTERRUPTS_HPP_
#define _ANDROS_INTERRUPTS_HPP_

#include "stdint.h"
#include "memory.hpp"

/* From the OSDev Wiki:
 *     7                           0
 *   +---+---+---+---+---+---+---+---+
 *   | P |  DPL  | S |    GateType   |
 *   +---+---+---+---+---+---+---+---+
 *   P : Present bit. Set to 0 for unused interrupts
 *   DPL : Descriptor Priviledge Level. Min level the calling descriptor should have.
 *   S   : Storage segment. Set to 0 for interrupt and trap gates
 *   Type: 0x5: 32-bit task gate
 *         0xE: 32-bit interrupt gate
 *         0xF: 32-bit trap gate
 */

#define IA32_INTERRUPT_GATE 0x8E
#define IA32_UNUSED_INTERRUPT_GATE 0x0E


struct IDTGate
{
  uint16_t offsetLow;
  uint16_t segmentSelector;
  uint8_t zero {0};
  uint8_t attributes {IA32_UNUSED_INTERRUPT_GATE};
  uint16_t offsetHigh;
} __attribute__((packed));

struct IDTPointer
{
  uint16_t size;
  uint32_t offset;
} __attribute__((packed));

struct InterruptFrame
{
  uint16_t ip;
  uint16_t cs;
  uint16_t flags;
  uint16_t sp;
  uint16_t ss;
} __attribute__((packed));

class IDT
{
public:
  IDT();
  IDTGate* m_gates;
  void writeToMemory();
  void registerInterruptHandler(int interrupt, void(*handler)(InterruptFrame* frame));
};

extern "C" void _loadIDT(IDTPointer* pointer);
extern "C" void testInterrupts(void);

extern "C" void isr_wrapper(void);
extern "C" int isrReturnValue;

#endif
