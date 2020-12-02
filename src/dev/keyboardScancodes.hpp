#ifndef _ANDROS_KEYBOARD_SCANCODES_HPP_
#define _ANDROS_KEYBOARD_SCANCODES_HPP_

#include "utils/CircularBuffer.hpp"

// TODO: A better way to do this.

#define NON_PRINTABLE_CHAR 0

enum ScancodeSet1_NonPrintables
{
  Escape = 0x1,
  Backspace = 0x0E,
  Tab = 0x0F,
  Enter = 0x1C,
  LControl = 0x1E,
  LeftShift = 0x2A,
  LeftAlt = 0x38

};

char ScancodeSet1_printableKeys [] = 
{
  NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, '1', '2', '3', '4', '5', '6', // 0x7
  '7', '8', '9', '0', '-', '=', NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, // 0xF
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', // 0x18
  'p', '[', ']', NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, 'a', 's', 'd', //0x20
  'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', // 0x28
  '`', NON_PRINTABLE_CHAR, '\\', 'z', 'x', 'c', 'v', 'b', // 0x30
  'n', 'm', ',', '.', '/', NON_PRINTABLE_CHAR, '*', NON_PRINTABLE_CHAR, // 0x38
  
};

bool ScancodeSet1_isPrintable(uint8_t scancode);

void processKeyboardInput(CircularBuffer<uint8_t>* keypresses, CircularBuffer<char> &outputBuffer);

#endif
