//#include "isrs/interruptHandlers.h"
//#include "pic/pic.hpp"
//#include "cpuIO.hpp"
#include "stdint.h"


#if 0
void keyboardHandler()
{
  //keyboardReadValue = inb(0x60);
  //sendEndOfInterrupt(1);
}
#endif

extern "C" uint32_t addOne(uint32_t value)
{
  return value + 1;
}
