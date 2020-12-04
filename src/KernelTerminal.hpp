#ifndef _ANDROS_KERNEL_TERMINAL_HPP_
#define _ANDROS_KERNEL_TERMINAL_HPP_

#include "utils/CircularBuffer.hpp"
#include "video/ConsoleScreen.hpp"
#include "mem/memory.hpp"

#define KERNEL_TERMINAL_BUFFER_SIZE 1024

class KernelTerminal
{
public:
  ConsoleScreen* m_stdout;
  CircularBuffer<char>* m_stdin;
  KernelTerminal();

  /* When called, parses input found in m_stdin and runs appropriate
   * actions on the input, runs shell functions, calls programs, etc. */
  void handleInput();
  
private:
  CircularBuffer<char> m_inputBuffer;

  /**************************************
   * SHELL FUNCTIONS
   **************************************/

  /* Prints "AndrOS v <version number>" */
  void printOSName();
};

#endif
