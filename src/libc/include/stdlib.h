#ifndef _ANDROS_LIBC_STDLIB_H
#define _ANDROS_LIBC_STDLIB_H

inline int atoi_char(char a)
{
  return (int)a - 48;
}

inline char itoa_char(int i)
{
  return (char)(i + 48);
}

int atoi(const char* str);

//It is the user's responsibility to ensure str is allocated
//with sufficient memory
void itoa(int value, char* str);
void itoa(int value, char* str, int base);

#endif
