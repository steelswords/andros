#include "stdint.h"
#include "interrupts.hpp"

int IDT::interruptReturnCode = 2;

//__attribute__((interrupt))
void defaultInterruptHandler(InterruptFrame* frame)
{
  //TODO: Maybe conditionally print something to the screen?
  IDT::interruptReturnCode = 26; // Just something so we know it is called.
}


IDT::IDT()
{
  m_gates = new IDTGate[256];
  for (int i = 0; i < 256; ++i)
  {
    uint32_t offset = (uint32_t) &defaultInterruptHandler;
    m_gates[i].offsetLow = (uint16_t)(offset & 0xFFFF);
    m_gates[i].offsetHigh = (uint16_t)((offset >> 16) & 0xFFFF);
    const int interruptSegmentSelector = 0x8;
    m_gates[i].segmentSelector = interruptSegmentSelector;
    m_gates[i].attributes = IA32_UNUSED_INTERRUPT_GATE;
  }
  IDT::interruptReturnCode = 32;
}

void IDT::writeToMemory()
{
  IDTPointer pointer;
  pointer.size = 256 * sizeof(IDTGate);
  pointer.offset = (uint32_t) &m_gates[0];
  _loadIDT(&pointer);
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

  writeToMemory();
  
}
