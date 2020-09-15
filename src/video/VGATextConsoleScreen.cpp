#include "video/ConsoleScreen.hpp"
#include "video/VGATextConsoleScreen.hpp"
#include <stdint.h>
#include "libc/include/stdlib.h"

VGATextConsoleScreen::VGATextConsoleScreen(ConsoleScreenParameters parameters)
  : ConsoleScreen(parameters),
    vga_memory((uint16_t*)VGAMemoryBase)
{
  setConsoleColors(parameters.backgroundColor, parameters.textColor);
}

void VGATextConsoleScreen::setConsoleColors(VGAColor backgroundColor, VGAColor textColor)
{
  ConsoleScreen::setConsoleColors(backgroundColor, textColor);
  m_consoleColor = textColor | backgroundColor << 4;
}

void VGATextConsoleScreen::setCursor(int16_t col, int16_t row)
{
  if (0 <= col && col < VGA_WIDTH)
  {
    m_column = col;
  }
  else
  {
    //TODO: Runtime error implementation.
  }

  if (0 <= row && row < VGA_HEIGHT)
  {
    m_row = row;
  }
  else
  {
    //TODO: Error implementation.
  }
}

void VGATextConsoleScreen::clear()
{
  uint16_t justColor;
 
  //Create a VGA entry that is just a space character with our
  //desired colors. 
  justColor = (uint16_t)' ' | m_consoleColor << 8; 

  // Wipe out console with the pre-set color.
  for (int y = 0; y < VGA_HEIGHT; ++y)
  {
    for (int x = 0; x < VGA_WIDTH; ++x)
    {
      int index = y * VGA_WIDTH + x;
      vga_memory[index] = justColor;
    }
  }    
}

void VGATextConsoleScreen::putChar(char c)
{
  putChar(c, m_column, m_row);
}
void VGATextConsoleScreen::putChar(char c, int16_t col, int16_t row)
{
  if (c == '\n')
  {
    m_row++;
    m_column = 0;
    return;
  }
  int index = row * VGA_WIDTH + col;
  uint16_t vgaData = c | m_consoleColor << 8;
  vga_memory[index] = vgaData;

  //Move the cursor to the next spot
  if (col + 1 >= VGA_WIDTH)
  {
    m_column = 0;
    m_row++;
  }
  else
  {
    m_column++;
  }
}

void VGATextConsoleScreen::print(char* str)
{
  int index = 0;
  while(str[index] != '\0')
  {
    putChar(str[index++], m_column, m_row);
  }
}

void VGATextConsoleScreen::print(int value)
{
  const int intStringLength = 11;
  char str[intStringLength];
  itoa(value, str, 10);
  print(str);
}

void VGATextConsoleScreen::printHex(int value)
{
  const int hexIntStringLength = 11;
  char str[hexIntStringLength];
  itoa(value, str, 16);
  print(str);
}

VGATextConsoleScreen::~VGATextConsoleScreen()
{
}
