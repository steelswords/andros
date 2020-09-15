#include "utils/utils.hpp"
#include "stdint.h"
#include "ConsoleScreen.hpp"

void memdump(ConsoleScreen* screen, void* addr, int length)
{
  uint8_t* data = (uint8_t*)addr;
  int index = 0;
  const int numCols = 8;
  int rows = length / numCols;
  for (int r = 0; r < rows; ++r)
  {
    screen->printHex(addr + index);
    screen->print(": ");
    for (int c = 0; c < numCols; ++r)
    {
      while (index < length)
      {
        screen->printHex(data[index]);
        screen->print(" ");
        index++;
      }
    }
    screen->putChar('\n');
  }
  for (int leftover = 0; leftover < length % numCols; ++leftover)
  {
    screen->printHex(data[index]);
    screen->print(" ");
    index++;
  }
  screen->putChar('\n');
}
