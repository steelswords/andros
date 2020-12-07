#include "KernelTerminal.hpp"

KernelTerminal::KernelTerminal()
  : m_inputBuffer(CircularBuffer<char>(KERNEL_TERMINAL_BUFFER_SIZE))
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
        break;
      case '\b':
      default:
        m_stdout->putChar(inputChar);
        break;
    }
  }
}
