#ifndef _ANDROS_X86CPU_HPP_
#define _ANDROS_X86CPU_HPP_

/******************************************************
 * Author: Tristan Andrus
 * Description: Various functions for interacting with an x86 CPU
 ******************************************************/

#include "stdint.h"

extern "C" uint32_t getEFlags();
extern "C" void setEFlags(uint32_t value);

#endif
