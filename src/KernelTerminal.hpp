#ifndef _ANDROS_KERNEL_TERMINAL_HPP_
#define _ANDROS_KERNEL_TERMINAL_HPP_

#include "utils/CircularBuffer.hpp"
#include "video/ConsoleScreen.hpp"
#include "mem/memory.hpp"

#define KERNEL_TERMINAL_BUFFER_SIZE 1024
#define KERNEL_TERMINAL_WIDTH 80
#define KERNEL_TERMINAL_HEIGHT 25

#define KERNEL_TERMINAL_COMMAND_BUFFER_SIZE 512

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
  //TODO: History for scrolling
  
  /* A buffer for potential commands to go in.
   * Will be replaced by a dynamic character array once I've made one */
  char m_commandBuffer[KERNEL_TERMINAL_COMMAND_BUFFER_SIZE] {0};
  int32_t m_commandBufferIndex {0};
  
  char m_screenContents[KERNEL_TERMINAL_HEIGHT][KERNEL_TERMINAL_WIDTH];

  uint16_t m_column;
  uint16_t m_row;
  void parseCommand();
  void backspace();



  /**************************************
   * SHELL FUNCTIONS
   **************************************/

  /* Prints "AndrOS v <version number>" */
  void printTerminalPrompt();
  void printOSName();
  void printMemoryMap();
};

#endif
