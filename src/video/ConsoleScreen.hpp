#ifndef _K_VIDEO__CONSOLE_SCREEN_H
#define _K_VIDEO__CONSOLE_SCREEN_H

#include <stdint.h>
#include "libc/include/stdlib.h"

/* Hardware text mode color constants. */
enum VGAColor {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

struct ConsoleScreenParameters
{
  VGAColor textColor;
  VGAColor backgroundColor;
};

/* Abstract Base Class for writing to Console. */
class ConsoleScreen
{
public:
  ConsoleScreen(ConsoleScreenParameters parameters)
    : m_textColor(parameters.textColor),
      m_backgroundColor(parameters.textColor)
    {}

  ConsoleScreen() {}

  virtual ~ConsoleScreen() {}
  
  /* Getters/Setters */
  virtual void setConsoleColors(VGAColor backgroundColor, VGAColor textColor)
  {
    m_textColor = textColor; 
    m_backgroundColor = backgroundColor;
  }

  inline virtual VGAColor getBackgroundColor() { return m_backgroundColor; }
  inline virtual VGAColor getTextColor() { return m_textColor; }

  inline virtual int getColumn() { return m_column; }
  inline virtual int getRow() { return m_row; }

  /* Sets the cursor */
  virtual void setCursor(int16_t col, int16_t row) = 0;

  /* Clears the console, filling it with whatever colors are already set. */
  virtual void clear() = 0;
  virtual void putChar(char c) = 0;
  virtual void putChar(char c, int16_t col, int16_t row) = 0;

  /* Writes a string at the current position. */
  virtual void print(char* str) = 0;

  /* Writes an int at the current position */
  virtual void print(int value) = 0;

  /* Writes an int in hexadecimal at the current position */
  virtual void printHex(int value) = 0;


protected:
  int16_t m_column;
  int16_t m_row;
  VGAColor m_textColor;
  VGAColor m_backgroundColor;
};
#endif
