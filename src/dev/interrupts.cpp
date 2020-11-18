#include "stdint.h"
#include "interrupts.hpp"
#include "memory.hpp"
#include "pic/pic.hpp"

__attribute__((interrupt))
void defaultInterruptHandler(InterruptFrame* frame)
{
  //TODO: Maybe conditionally print something to the screen?
}


IDT::IDT()
{
}

void IDT::init()
{
#if 1
  //TODO: Do I need to allocate this memory?
  //m_gates =(IDTGate*) malloc(sizeof(IDTGate) * NUM_IDT_GATES);
  //m_gates = new IDTGate[256];
  for (int i = 0; i < NUM_IDT_GATES; ++i)
  {
    uint32_t offset = (uint32_t) (&defaultISR);
    m_gates[i].offsetLow = (uint16_t)(offset & 0xFFFF);
    m_gates[i].offsetHigh = (uint16_t)((offset >> 16) & 0xFFFF);
    const int interruptSegmentSelector = 0x8; //TODO: Check this boi
    m_gates[i].segmentSelector = interruptSegmentSelector;
    if (i > 31)
      m_gates[i].attributes = IA32_INTERRUPT_GATE;
    else
      m_gates[i].attributes = IA32_TRAP_GATE;
  }
#endif
	remapPIC(IRQ_VECTOR_OFFSET, IRQ_VECTOR_OFFSET_PIC2);
}

void IDT::load()
{
#if 0
	lidt(&m_gates[0], 256 * sizeof(IDTGate));
#endif
#if 1
  IDTPointer pointer;
  pointer.size = 256 * sizeof(IDTGate);
  pointer.offset = (uint32_t) &m_gates[0];
  _loadIDT(&pointer);
#endif
}

void IDT::registerInterruptHandler(int interrupt, void(*handler)(InterruptFrame* frame))
{
  //TODO: This.
  uint32_t offset = (uint32_t) &handler;
  m_gates[interrupt].offsetLow = (uint16_t)(offset & 0xFFFF);
  m_gates[interrupt].offsetHigh = (uint16_t)((offset >> 16) & 0xFFFF);
  const int interruptSegmentSelector = 0x8;
  m_gates[interrupt].segmentSelector = interruptSegmentSelector;
  m_gates[interrupt].attributes = IA32_UNUSED_INTERRUPT_GATE;

  load();
  
}
