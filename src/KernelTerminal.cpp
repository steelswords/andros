#include "KernelTerminal.hpp"

KernelTerminal::KernelTerminal()
  : m_inputBuffer(CircularBuffer<char>(KERNEL_TERMINAL_BUFFER_SIZE))
{
  m_stdin = & m_inputBuffer;
}

static bool isControlChar(char c)
{
  if (c != '\b')
  {
    return false;
  }
}

void KernelTerminal::handleInput()
{
  while(!m_stdin->isEmpty())
  {
    char inputChar = m_stdin->get();
    if (!isControlChar(inputChar))
    {
      m_stdout->putChar(inputChar);
    }
  }
}
