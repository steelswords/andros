#include "KernelTerminal.hpp"
#include "utils/kstring.hpp"

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
  if (kstring::isEqual(m_commandBuffer, greetingCommand))
  {
    m_stdout->print("Hello");
  }
  if (kstring::isEqual(m_commandBuffer, printMemMapCommand))
  {
  }

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
        m_stdout->print("Command entered: ");
        m_stdout->print((char*)m_commandBuffer);
        m_stdout->nl();

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
        //if (m_commandBufferIndex < 512)
        {
          m_commandBuffer[m_commandBufferIndex] = inputChar;
          m_commandBufferIndex++;
        }
        m_stdout->putChar(inputChar);
        break;
    }
  }
}

void KernelTerminal::printMemoryMap()
{
  //System::getInstance()->mbhi.printMemoryTable(m_stdout);
}

void KernelTerminal::printTerminalPrompt()
{
  m_stdout->print("andros > ");
}
