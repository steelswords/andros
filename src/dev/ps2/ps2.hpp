#ifndef _ANDROS_PS_2_HPP_
#define _ANDROS_PS_2_HPP_

#include "stdint.h"


// Read only
#define PS2_STATUS_PORT 0x64 

//Write only
#define PS2_COMMAND_PORT 0x64

// Read/Write
#define PS2_DATA_PORT 0x60

// Commands
#define PS2_CMD_TEST_CONTROLLER 0xAA
#define PS2_CMD_ENABLE_PS2_PORT_1 0xAE
#define PS2_CMD_DISBLE_PS2_PORT_1 0xAD

// Status Register
#define PS2_STATUS_OUTPUT_BUFFER_STATUS_BIT 0 // 0 = empty, 1 = full
#define PS2_STATUS_INPUT_BUFFER_STATUS_BIT  1 // 0 = empty, 1 = full
#define PS2_STATUS_SYSTEM_FLAG              2

class PS2Keyboard
{
public:
  static uint8_t testPS2Controller();
  static uint8_t readStatusRegister();
  static char readByte();
};


#endif
