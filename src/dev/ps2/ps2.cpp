#include "stdint.h"
#include "ps2.hpp"
#include "dev/cpuIO.hpp"

uint8_t PS2Keyboard::testPS2Controller()
{
  outb(PS2_COMMAND_PORT, PS2_CMD_TEST_CONTROLLER);
  uint8_t reply = inb(PS2_STATUS_PORT);
  return reply;
}

uint8_t PS2Keyboard::readStatusRegister()
{
  return inb(PS2_STATUS_PORT);
}

char PS2Keyboard::readByte()
{
  return inb(PS2_DATA_PORT);
}

void PS2Keyboard::flushBuffer()
{
  char oldValue = 0;
  while (1)
  {
    char value = inb(PS2_DATA_PORT);
    if (value == oldValue)
    break;
  }
}
