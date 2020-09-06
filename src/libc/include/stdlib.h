#ifndef _ANDROS_LIBC_STDLIB_H
#define _ANDROS_LIBC_STDLIB_H

inline int atoi(char a)
{
  return (int)a - 48;
}

int atoi(const char* str);

inline char itoa(int i)
{
  return (char)(i + 48);
}

//It is the user's responsibility to ensure str is allocated
//with sufficient memory
void itoa(int value, char* str);


#endif
