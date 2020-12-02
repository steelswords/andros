#ifndef _ANDROS_INTERRUPTS_HPP_
#define _ANDROS_INTERRUPTS_HPP_

#include "stdint.h"
#include "isrs/interruptHandlers.hpp"

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
#define IA32_TRAP_GATE 0x8F

#define NUM_IDT_GATES 256

#define IRQ_VECTOR_OFFSET 0x70
#define IRQ_VECTOR_OFFSET_PIC2 (IRQ_VECTOR_OFFSET + 8)

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
  //IDTGate m_gates[NUM_IDT_GATES];
  IDTGate* m_gates;
  void registerInterruptHandler(int interrupt, void(*handler)(InterruptFrame* frame));
  void init(); // Loads default ISR into all the gates
  void load(); // loads the IDT into memory and calls lidt
  void registerInterruptHandler(int interrupt, void(*handler)());
  void initISRs(); // Loads the various ISRs we need to make things work.
};

/* The following code courtesy of wiki.osdev.org */
/*
static inline void lidt(void* base, uint16_t size)
{   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };
 
    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}
*/

extern "C" void _loadIDT(IDTPointer* pointer);
extern "C" void testInterrupts(void);

extern "C" void isr_wrapper(void);
extern "C" int isrReturnValue;

extern "C" int interruptReturnValue;
extern "C" void testInterrupts();
extern "C" void enableInterruptFlag();

// Interrupt Handlers
extern "C" void defaultISR();
extern "C" void keyboardISR();

extern "C" int keyboardScancode;

#endif
