#include "KernelTerminal.hpp"

KernelTerminal::KernelTerminal()
  : m_inputBuffer(CircularBuffer<char>(KERNEL_TERMINAL_BUFFER_SIZE)),
    m_commandBufferIndex(0)
{
  m_stdin = & m_inputBuffer;
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
#if 0
        m_commandBuffer[0] = 'q';
        m_commandBuffer[1] = 'u';
        m_commandBuffer[2] = 'a';
        m_commandBuffer[3] = 'b';
        m_commandBuffer[4] = 'i';
        m_commandBuffer[5] = 't';
        m_commandBuffer[6] = 'y';
        m_commandBuffer[7] = '\0';
#endif
        
        m_stdout->print((char*)m_commandBuffer);
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
