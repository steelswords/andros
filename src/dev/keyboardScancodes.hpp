#ifndef _ANDROS_KEYBOARD_SCANCODES_HPP_
#define _ANDROS_KEYBOARD_SCANCODES_HPP_

#include "utils/CircularBuffer.hpp"
#include "stdint.h"

#define NON_PRINTABLE_CHAR 0

// TODO: A better way to do this.
// Some issues: There is no state capture. There is no distinguishing between keypad and keyboard numbers.


enum ScancodeSet1_NonPrintables
{
  Escape = 0x1,
  Backspace = 0x0E,
  Tab = 0x0F,
  Enter = 0x1C,
  LeftControl = 0x1E,
  LeftShift = 0x2A,
  LeftAlt = 0x38,
  CapsLock = 0x3A,
  F1 = 0x3B,
  F2 = 0x3C,
  F3 = 0x3D,
  F4 = 0x3E,
  F5 = 0x3F,
  F6 = 0x40,
  F7 = 0x41,
  F8 = 0x42,
  F9 = 0x43,
  F10 = 0x44,
  F11 = 0x57,
  F12 = 0x58,
  NumLock = 0x45,
  ScrollLock = 0x46
};


extern char ScancodeSet1_printableKeys[];

bool ScancodeSet1_isPrintable(uint8_t scancode);

void processKeyboardInput(CircularBuffer<uint8_t>* keypresses, CircularBuffer<char> *outputBuffer);

#endif
