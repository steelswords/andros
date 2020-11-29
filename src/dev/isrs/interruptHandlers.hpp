#ifndef _ANDROS_INTERRUPT_HANDLERS_H_
#define _ANDROS_INTERRUPT_HANDLERS_H_


#include "stdint.h"

// Convention: The assembly wrappers will be called *ISR_wrapper,
// and the actual interrupt handler in C shall be known by the
// more reader-friendly *Handler.
extern "C" void keyboardHandler();
extern "C" void timerHandler();

extern uint32_t keyboardReadValue;



extern "C" uint32_t addOne(uint32_t value);
extern "C" uint32_t addTwo(uint32_t value);

extern "C" void keyboardISR_wrapper(void);
extern "C" void timerISR_wrapper(void);


#endif
