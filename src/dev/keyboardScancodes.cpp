#include "dev/keyboardScancodes.hpp"

char ScancodeSet1_printableKeys [] = 
{
  NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, '1', '2', '3', '4', '5', '6', // 0x7
  '7', '8', '9', '0', '-', '=', NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, // 0xF
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', // 0x18
  'p', '[', ']', NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, 'a', 's', 'd', //0x20
  'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', // 0x28
  '`', NON_PRINTABLE_CHAR, '\\', 'z', 'x', 'c', 'v', 'b', // 0x30
  'n', 'm', ',', '.', '/', NON_PRINTABLE_CHAR, '*', NON_PRINTABLE_CHAR, // 0x38
  ' ', NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, // F3, 0x3D,
  NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR,     // F7, 0x41
  NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR,     // 0x45
  NON_PRINTABLE_CHAR, '7', '8', '9', '-', '4', '5', '6', '+', '1', '2',  // 0x50
  '3', '0', '.', NON_PRINTABLE_CHAR, NON_PRINTABLE_CHAR // 0x58 
};

bool ScancodeSet1_isPrintable(uint8_t scancode)
{
  // Returns true if scancode is a printable char, i.e. 
  // it doesn't return a NON_PRINTABLE_CHAR value
  return ScancodeSet1_printableKeys[scancode] != NON_PRINTABLE_CHAR;
}


void processKeyboardInput(CircularBuffer<uint8_t>* keypresses, CircularBuffer<char>* outputBuffer)
{
  static bool shiftOn = false;
  static bool controlOn = false;
  static bool altOn = false;
  while (!keypresses->isEmpty())
  {
    //Consume some keys
    uint8_t keypress = keypresses->get();
    
    // First, if it is a release keypress, than subtract 0x80 to find the correct
    // key
    if (keypress >= 0x80)
    {
      uint8_t adjKeypress = keypress - 0x80;
      // If it is a a release and a printable, 
      if (ScancodeSet1_isPrintable(adjKeypress))
      {
        // Add the character to outputBuffer
        char printableChar = ScancodeSet1_printableKeys[adjKeypress];
        if (shiftOn)
        {
          printableChar -= 32;
        }
        outputBuffer->add(printableChar);
      }
      // If it's not printable, but is a release,
      else
      {
        // Switch through cases and update state as necessary
        switch(adjKeypress)
        {
          case LeftShift:
            shiftOn = false;
            break;
          case LeftControl:
            controlOn = false;
            break;
          case LeftAlt:
            altOn = false;
            break;
          case Backspace:
            outputBuffer->add('\b');
            break;
        }
      }
        
    }
    
    // Else, if it is a downpress keypress
    else
    {
      // If it is a press and printable, don't do anything.
      if (ScancodeSet1_isPrintable(keypress))
      {
      }
      // If not a printable, set some state
      else
      {
        // Switch on key, set state.
        switch(keypress)
        {
          case LeftShift:
            shiftOn = true;
            break;
          case LeftControl:
            shiftOn = true;
            break;
          case LeftAlt:
            altOn = true;
            break;

        }
      }
      
    }
  } // While
}
