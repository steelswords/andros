#ifndef _ANDROS_UTILS_H_
#define _ANDROS_UTILS_H_
#include "stdint.h"
#include "ConsoleScreen.hpp"

void memdump(ConsoleScreen* screen, void* addr, int length);
extern "C" uint32_t byteswap32(uint32_t value);

#endif
