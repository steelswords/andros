#include "dev/keyboardScancodes.hpp"

bool ScancodeSet1_isPrintable(uint8_t scancode)
{
  // Returns true if scancode is a printable char, i.e. 
  // it doesn't return a NON_PRINTABLE_CHAR value
  return ScancodeSet1_printableKeys[scancode] != NON_PRINTABLE_CHAR;
}

void processKeyboardInput(CircularBuffer<uint8_t>* keypresses, CircularBuffer<char> &outputBuffer)
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
    if ( keypress >= 0x80)
    {
      uint8_t adjKeypress = keypress - 0x80;
      // If it is a a release and a printable, 
      if (ScancodeSet1_isPrintable(adjKeypress))
      {
        // Add the character to outputBuffer
        char printableChar = ScancodeSet1_printableKeys[adjKeypress];
        outputBuffer.add(printableChar);
      }
      // If it's not printable, but is a release,
      else
      {
        // Switch through cases and update state as necessary
        switch(adjKeypress)
        {

        }
      }
        
    }
    
    // Else, if it is a press keypress
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
      }
      
    }
  }

}
