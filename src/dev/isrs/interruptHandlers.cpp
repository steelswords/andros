#include "isrs/interruptHandlers.hpp"
#include "pic/pic.hpp"
#include "cpuIO.hpp"
#include "stdint.h"
#include "System.hpp"


extern "C" void keyboardHandler()
{
  //TODO: Put a circular buffer here.
  keyboardReadValue = inb(0x60);
  sendEndOfInterrupt(1);
#if 0
  System* sys = System::getInstance();
  sys->screen->print("[");
  sys->screen->print(keyboardReadValue);
  sys->screen->print("] ");
#endif
}

extern "C" uint32_t addOne(uint32_t value)
{
  return value + 1;
}
