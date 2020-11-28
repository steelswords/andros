#ifndef _ANDROS_INTERRUPT_HANDLERS_H_
#define _ANDROS_INTERRUPT_HANDLERS_H_


#include "stdint.h"

#if 1
extern "C" void keyboardHandler(struct interrupt_frame* frame);

uint32_t keyboardReadValue;

#endif


extern "C" uint32_t addOne(uint32_t value);
extern "C" uint32_t addTwo(uint32_t value);

#endif
