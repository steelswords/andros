#include <iostream>
#include <climits>
#include <cstddef>
#include <assert.h>
#include <sstream>
#include <cstring>
#include <string>
namespace test
{
#define __UNIT_TEST__ 1
#define _DEBUG_ 1
#include "libc/stdlib.c"
}

int main()
{
  std::stringstream builder;
  int integers [] = {1, 0, -1, -0, 1234567809, INT_MAX, INT_MIN + 1}; //Was overflowing

  std::cout << "----------------------------\n";
  std::cout << "  atoi Unit Test" << std::endl;
  std::cout << "----------------------------\n";


  std::string a = "1234";
  char* b = "1234";
  assert(strcmp(a.c_str(), b) == 0);

  for (int i =0; i < 7; ++i)
  {
    builder.str("");
    builder << integers[i];
    char testedString[11]; // 10 digits, 1 negative sign
    test::itoa(integers[i], testedString);
    std::cout << "Should be " << integers[i] << ": " << testedString << std::endl;
    
    assert(strcmp(builder.str().c_str(), testedString) == 0);
  }

  std::cout << "All good!" << std::endl;
  return 0;
}
