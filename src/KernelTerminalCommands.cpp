#include "KernelTerminal.hpp"
#include "utils/kstring.hpp"
#include "System.hpp"
#include "stddef.h"

void KernelTerminal::peek(uint32_t address)
{
  // Get integer for first argument
  // Read value and
  // Display
  m_stdout->print("Memory location at ");
  m_stdout->printHex(address);
  m_stdout->print(": ");
  uint8_t value = *(uint8_t*)address;
  m_stdout->printHex(value);
}

void KernelTerminal::poke(uint8_t* address, uint8_t value)
{
  m_stdout->print("Changing location ");
  m_stdout->printHex((uint32_t)address);
  m_stdout->print(" to ");
  m_stdout->printHex(value);
  *address = value;
}
