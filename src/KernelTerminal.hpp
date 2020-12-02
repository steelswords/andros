#ifndef _ANDROS_KERNEL_TERMINAL_HPP_
#define _ANDROS_KERNEL_TERMINAL_HPP_

#include "utils/CircularBuffer.hpp"
#include "video/ConsoleScreen.hpp"

//TODO: Put this in another file
typedef char Keystroke;

class KernelTerminal
{
public:
  ConsoleScreen* m_stdout;
  CircularBuffer<Keystroke>* m_stdin;

  /* When called, parses input found in m_stdin and runs appropriate
   * actions on the input, runs shell functions, calls programs, etc. */
  void handleInput();
  
private:

  /**************************************
   * SHELL FUNCTIONS
   **************************************/

  /* Prints "AndrOS v <version number>" */
  void printOSName();
};

#endif
