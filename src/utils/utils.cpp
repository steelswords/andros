#include "utils/utils.hpp"
#include "stdint.h"
#include "ConsoleScreen.hpp"
#include "kstring.hpp"

void memdump(ConsoleScreen* screen, void* addr, int length)
{
  screen->print("DUMPING ");
  screen->printHex((uint32_t)addr);
  screen->nl();

  uint8_t* data = (uint8_t*)addr;
  int index = 0;
  const int numCols = 8;
  int rows = length / numCols;
  for (int r = 0; r < rows; ++r)
  {
    screen->printHex((uint64_t)addr + index);
    screen->print(": ");
    int lastConsoleCol = screen->getColumn();
    for (int c = 0; c < numCols; ++c)
    {
      if (index < length)
      {
        screen->printHex(data[index]);
        
        //If the difference between last column and this column is less than five,
        //print out spaces until it is.
        while ((screen->getColumn() - lastConsoleCol) < 5)
        {
          screen->putChar(' ');
        }
        lastConsoleCol = screen->getColumn();

        index++;
      }
    }
    screen->putChar('\n');
  }
  if (length % numCols) // If there is leftover, print the beginning address part again
  {
    screen->printHex((uint64_t)addr + index);
  }
  for (int leftover = 0; leftover < length % numCols; ++leftover)
  {
    screen->printHex(data[index]);
    screen->print(" ");
    index++;
  }
  screen->putChar('\n');
}
