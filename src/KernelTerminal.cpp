#include "KernelTerminal.hpp"
#include "mem/memory.hpp"

KernelTerminal::KernelTerminal()
{
  m_stdin = new CircularBuffer<char>(KERNEL_TERMINAL_BUFFER_SIZE);
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
