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
  char greetingCommand[] = {"greet"};
  char printMemMapCommand[]  = "memmap";
  char peekCommand[]         = "peek";
  char pokeCommand[]         = "poke";
  char cpuidCommand[]        = "cpuid";
  char osnameCommand[]       = "osname";
  char mallocCommand[]       = "malloc"; 

  if (kstring::isEqual(m_commandBuffer, greetingCommand))
  {
    m_stdout->print("Hello, user!\n");
  }
  else if (kstring::isEqual(m_commandBuffer, printMemMapCommand))
  {
    printMemoryMap();
  }
  /* Peek */
  else if (kstring::startsWith(m_commandBuffer, peekCommand))
  {
    //const int sizeOfPeekCommand = 4;
    //m_commandBufferIndex += sizeOfPeekCommand;
    size_t throwaway = 0;
    uint32_t address = getInt(((char*)m_commandBuffer) + 4, throwaway);
    peek(address); 
  }
  /* Poke */
  else if (kstring::startsWith(m_commandBuffer, pokeCommand))
  {
    // This is just getting an int from m_commandBuffer + 4
    // (The offset for 'poke').
    size_t lengthOfAddress = 0;
    char* argumentBuffer = ((char*)m_commandBuffer) + 4;

    uint8_t* address = (uint8_t*) getInt(argumentBuffer, lengthOfAddress);
    argumentBuffer += lengthOfAddress;
    
    uint8_t value = (uint8_t)getInt(argumentBuffer, lengthOfAddress);
    poke(address, value);
  }

  else if (kstring::isEqual(m_commandBuffer, cpuidCommand))
    this->cpuid();
  else if (kstring::isEqual(m_commandBuffer, osnameCommand))
    this->printOSName();
  else if (kstring::isEqual(m_commandBuffer, mallocCommand))
  {
    size_t argLength = 0;
    uint32_t sizeToAllocate = 0;
    char* argumentBuffer = ((char*)m_commandBuffer) + 6;
    sizeToAllocate = getInt(argumentBuffer, argLength);
    demoMalloc(sizeToAllocate);
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
#if 0
        //DEBUG
        m_stdout->print("Command entered: ");
        m_stdout->print((char*)m_commandBuffer);
        m_stdout->nl();
#endif

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

void KernelTerminal::printTerminalPrompt()
{
  m_stdout->print("andros> ");
}

uint32_t KernelTerminal::getInt(char* input, size_t &charactersConsumed)
{
  // Set up
  uint32_t result = 0;
  size_t index = 0;
  size_t numDigits = 0;

  // First, skip until we reach a hex character
  while (input[index] != '\0' && isHexDigit(input[index]))
  {
    index++;
    charactersConsumed++;
  }

  //Check if the number starts with '0x'
  if (input[index] != '\0' && input[index + 1] != '\0')
  {
    if (input[index] == '0' && (input[index + 1] == 'x' || input[index + 1] == 'X'))
    {
      // Just get rid of those characters
      index += 2; 
      charactersConsumed += 2;
    }
  }

  // Now we can pass this into toIntFromHex.
  result = kstring::toIntFromHex(&input[index], numDigits);
  
  // Return results
  charactersConsumed += numDigits;
  return result;
}
