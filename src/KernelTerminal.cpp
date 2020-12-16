#include "KernelTerminal.hpp"
#include "utils/kstring.hpp"
#include "System.hpp"
#include "stddef.h"

KernelTerminal::KernelTerminal()
  : m_inputBuffer(CircularBuffer<char>(KERNEL_TERMINAL_BUFFER_SIZE)),
    m_commandBufferIndex(0)
{
  m_stdin = & m_inputBuffer;
}

void KernelTerminal::parseCommand()
{
  char* greetingCommand = "greet";
  char* printMemMapCommand  = "memmap";
  char* peekCommand         = "peek";

  if (kstring::isEqual(m_commandBuffer, greetingCommand))
  {
    m_stdout->print("Hello, user!\n");
  }
  else if (kstring::isEqual(m_commandBuffer, printMemMapCommand))
  {
    System::getInstance()->mbhi.printMemoryTable(m_stdout);
    m_stdout->nl();
  }
  /* Peek */
  else if (kstring::startsWith(m_commandBuffer, peekCommand))
  {
    //const int sizeOfPeekCommand = 4;
    //m_commandBufferIndex += sizeOfPeekCommand;
    uint32_t address = getInt(((char*)m_commandBuffer) + 4);
    peek(address); 
  }

  // Unrecognized command
  else
  {
    m_stdout->print("Unrecognized command: ");
    m_stdout->print(m_commandBuffer);
    m_stdout->nl();
  }

  // TODO: Reset m_commandBuffer to 0 and index to 0?
  for (int i = 0; i < KERNEL_TERMINAL_COMMAND_BUFFER_SIZE; ++i)
  {
    m_commandBuffer[i] = 0;
  }
  m_commandBufferIndex = 0;
  m_stdout->nl();
  printTerminalPrompt();
}

void KernelTerminal::handleInput()
{
  while(!m_stdin->isEmpty())
  {
    char inputChar = m_stdin->get();
    switch(inputChar)
    {
      case '\n':
        //TODO: Parse & execute command
        m_stdout->nl();
        //m_stdout->print("Command entered: ");
        //m_stdout->print((char*)m_commandBuffer);
        //m_stdout->nl();

        parseCommand();

        // Clear command buffer
        // TODO: Add to shell history
        for (; m_commandBufferIndex >0; --m_commandBufferIndex)
        {
          m_commandBuffer[m_commandBufferIndex] = 0;
        }
        break;
      case '\b':
        // Remove last character from commandBuffer, if there is one
        if (m_commandBufferIndex > 0)
        {
          m_commandBuffer[m_commandBufferIndex] = '\0';
          m_commandBufferIndex--;
        }

        // Print the backspace to the screen
        m_stdout->putChar(inputChar);
        break;
      // All other characters
      default:
        // If the commandBuffer is not full, add to end of it
        if (m_commandBufferIndex < KERNEL_TERMINAL_COMMAND_BUFFER_SIZE)
        {
          m_commandBuffer[m_commandBufferIndex] = inputChar;
          m_commandBufferIndex++;
        }
        m_stdout->putChar(inputChar);
        break;
    }
  }
}

void KernelTerminal::peek(uint32_t address)
{
  // Get integer for first argument
  // Read value and
  // Display
  m_stdout->print("Memory location at ");
  m_stdout->printHex(address);
  //m_stdout->printHex(getInt());
}

void KernelTerminal::printTerminalPrompt()
{
  m_stdout->print("andros> ");
}

uint32_t KernelTerminal::getInt(char* cmd)
{
  //First, skip whitespace
  //char* cmd = &m_commandBuffer[m_commandBufferIndex];
  //char* cmd = m_commandBuffer;
  //cmd += 4;
  cmd = kstring::skipWhitespace(cmd);
  m_stdout->print("cmd = ");
  m_stdout->print(cmd);
  m_stdout->print(". m_commandBuffer = ");
  m_stdout->print(m_commandBuffer);


  //Sort through until we have a break in the number.
  const int maxDigitsInInt = 12;
  char integerAsStringBuffer[maxDigitsInInt] = {0};
  //char integerAsStringBuffer[maxDigitsInInt] = "1234";
  int integerAsStringBufferIndex = 0;

#if 1
  // If we can, get an initial char from m_stdin
  // Loop while the char is not a hexadecimal int
  bool haveHitNumberCharactersYet = false;
  for (int i = 0; cmd[i] != '\0'; ++i)
  {
    if (isHexDigit(cmd[i]))
    {
      haveHitNumberCharactersYet = true;
      if (integerAsStringBufferIndex + 1 >= maxDigitsInInt)
      {
        // Gotcha! Buffer overflow averted
        break;
      }
      integerAsStringBuffer[integerAsStringBufferIndex] = cmd[i];
      integerAsStringBufferIndex++;
    }
    else //Not a hex digit
    {
      if (haveHitNumberCharactersYet)
      {
        // This means we have passed the integer.
        // Update the commandBuffer Index and break out of loop.
        m_commandBufferIndex += i;
        break; 
      }
    }
  }
#endif

  size_t numLength = 0;
#if 0
  m_stdout->print("m_commandBuffer = ");
  m_stdout->print(m_commandBuffer);
  m_stdout->print(".\n");
  m_stdout->print("cmd = ");
  m_stdout->print(cmd);
#endif
  m_stdout->print(". Translating ");
  m_stdout->print((char*)integerAsStringBuffer);
  m_stdout->print(" to int.\n");
  return (uint32_t)kstring::toIntFromHex((char*)integerAsStringBuffer, numLength);
}
