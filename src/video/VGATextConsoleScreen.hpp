#ifndef _K_VIDEO_VGA_TEXT_CONSOLE_SCREEN_H_
#define _K_VIDEO_VGA_TEXT_CONSOLE_SCREEN_H_

#include <stdint.h>

static const int VGAMemoryBase = 0xB8000;
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

#include "video/ConsoleScreen.hpp"
#include "utils/kstring.hpp"

void operator delete(void* target);

class VGATextConsoleScreen : public ConsoleScreen
{
public:
  VGATextConsoleScreen(ConsoleScreenParameters parameters);
  VGATextConsoleScreen() : vga_memory((uint16_t*)VGAMemoryBase) {}
  virtual ~VGATextConsoleScreen();

  virtual void setConsoleColors(VGAColor backgroundColor, VGAColor textColor);

  inline virtual VGAColor getBackgroundColor() { return m_backgroundColor; }
  inline virtual VGAColor getTextColor() { return m_textColor; }

  inline virtual int getColumn() { return m_column; }
  inline virtual int getRow() { return m_row; }

  /* Sets the cursor */
  virtual void setCursor(int16_t col, int16_t row);

  virtual void clear();

  virtual void putChar(char c);
  virtual void putChar(char c, int16_t col, int16_t row);

  /* Writes a string at the current position. */
  virtual void print(char* str);
  virtual void print(kstring message);
  virtual void print(int value);
  virtual void printHex(uint32_t value);
  virtual void printlHex(uint64_t value);
  
  virtual void nl();

private:
  uint16_t* vga_memory; // C-style for C-style programming tricks.
  uint8_t m_consoleColor; // A combination of fore- and background text, bg in high nibble, fg in low nibble.
};

#endif
