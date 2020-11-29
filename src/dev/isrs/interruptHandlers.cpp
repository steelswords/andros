#include "isrs/interruptHandlers.hpp"
#include "pic/pic.hpp"
#include "ps2/ps2.hpp"
#include "cpuIO.hpp"
#include "stdint.h"
#include "System.hpp"

uint32_t keyboardReadValue = 0;

extern "C" void keyboardHandler()
{
  return;
  //TODO: Put a circular buffer here.
  keyboardReadValue = inb(0x60);
  sendEndOfInterrupt(1);
  return;
#if 1
  ConsoleScreen* screen = System::getInstance()->screen;
  screen->print("asdf");
  screen->print("[");
  screen->print(keyboardReadValue);
  screen->print("] ");
#endif
  sendEndOfInterrupt(1);
}

extern "C" void timerHandler()
{
  sendEndOfInterrupt(0);
}

extern "C" uint32_t addOne(uint32_t value)
{
  return value + 1;
}
