#include "stdint.h"
#include "interrupts.hpp"
#include "memory.hpp"
#include "pic/pic.hpp"


#if 0
//__attribute__((interrupt))
void defaultInterruptHandler(InterruptFrame* frame)
{
  //TODO: Maybe conditionally print something to the screen?
  IDT::interruptReturnCode = 26; // Just something so we know it is called.
}
#endif



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
  initISRs();
}

void IDT::initISRs()
{
  // Initialize all the ISRs in general use, i.e. keyboard handler.
  //enableIRQ(0);
  disableIRQ(0);

  // Keyboard handler
  // For now we are going to disable the keyboard IRQ until we get that
  // ISR problem figured out.
  // enableIRQ(1);
  disableIRQ(1); 
  this->registerInterruptHandler(IRQ_VECTOR_OFFSET, &timerISR_wrapper);
  this->registerInterruptHandler(IRQ_VECTOR_OFFSET+1, &keyboardISR_wrapper);
  enableInterruptFlag();
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

// TODO: Overload for C++ functions with a stack input once we can write ISRs in C++
void IDT::registerInterruptHandler(int interrupt, void(*handler)(void))
{
  uint32_t offset = (uint32_t) &handler;
  m_gates[interrupt].offsetLow = (uint16_t)(offset & 0xFFFF);
  m_gates[interrupt].offsetHigh = (uint16_t)((offset >> 16) & 0xFFFF);
  const int interruptSegmentSelector = 0x8;
  m_gates[interrupt].segmentSelector = interruptSegmentSelector;
  m_gates[interrupt].attributes = IA32_INTERRUPT_GATE;

  //TODO: Is this necessary?
  load();
}
