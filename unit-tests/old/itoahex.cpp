#include <iostream>
#include <climits>
#include <cstddef>
#include <assert.h>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>

namespace test
{
#define __UNIT_TEST__ 1
#define _DEBUG_ 1
#include "libc/stdlib.c"
}

int main()
{
  std::stringstream builder;
  std::stringstream parser, parser2;
  std::vector<unsigned int> integers = {1, 0, (unsigned int)-1, -0, 1234567809u, INT_MAX, /*(unsigned)INT_MIN + 1,*/ 0x123456, (unsigned)0xabcdef20u, (unsigned)0xbadbeefu,
  0x1ceb00da, (unsigned)0xb105f00d, 0x2bad4ded}; // INT_MIN Was overflowing
  int a = 0;
  int b = 0;

  std::cout << "----------------------------\n";
  std::cout << "  itoa (hex) Unit Test" << std::endl;
  std::cout << "----------------------------\n";

  std::string aStr = "0x1ceb00b5";
  char bStr[]      = "0x1ceb00b5";
  a = std::stoi(aStr, 0, 16);
  b = std::stoi(bStr, 0, 16);

  std::cout << "a = " << std::hex << a << ". b = " << b << std::endl;
  assert(a == b);
  
  //std::cout << std::stoi("-1", 0, 16);

  for (unsigned int i : integers)
  {
    std::cout << "Should be " << std::hex << i << ": " << std::flush;
    unsigned long testValue = 0;
    char testString[11] = {0};
    test::itoa((int)i, testString, 16);
    //Convert testString back into an int.
    testValue = std::stoul(testString, 0, 16);
    std::cout << testValue 
              << "(" << testString << ")" 
              << std::endl;
    assert(testValue == i);
  }

  std::cout << "All good!" << std::endl;
  return 0;
}
