#ifndef _ANDROS_KEYBOARD_HPP
#define _ANDROS_KEYBOARD_HPP

class Keyboard
{
public:
#if 0 
  virtual void setCapsLockLED(bool on);
  virtual void setNumLockLED(bool on);
  virtual void setScrollLockLED(bool on);
#endif

};

extern "C" void keyboardInterruptHandler();

//TODO: I think what I need to do is first, make a wrapper ISR in asm.
//It will push the general registers and call this function.
//I need to implement this in the Makefile.
//In this handler, I need to read a byte from the keyboard.
//
#endif
