#include "isrs/interruptHandlers.hpp"
#include "pic/pic.hpp"
#include "cpuIO.hpp"
#include "stdint.h"
#include "System.hpp"

uint32_t keyboardReadValue = 0;

extern "C" void keyboardHandler()
{
  //TODO: Put a circular buffer here.
  keyboardReadValue = inb(0x60);
  sendEndOfInterrupt(1);
#if 1
  ConsoleScreen* screen = System::getInstance()->screen;
  screen->print("asdf");
  screen->print("[");
  screen->print(keyboardReadValue);
  screen->print("] ");
#endif
}

extern "C" void timerHandler()
{
  sendEndOfInterrupt(1);
}

extern "C" uint32_t addOne(uint32_t value)
{
  return value + 1;
}
