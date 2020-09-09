#include <iostream>
#include <cstddef>
#include <limits>
#include <assert.h>
#include <sstream>
#include <string>
#include <vector>

namespace test
{
#define __UNIT_TEST__
#include "libc/string.c"
}

int main()
{
  std::vector<std::string> testStrings = {"Hello", "Mornie alantie",
    "Kyrie Elesion", "1234", "Me gusta bailar en la primavera, pepino"};

  std::cout << "------------------------\n";
  std::cout << "    strlen Unit Test\n";
  std::cout << "------------------------\n";
  for (auto str : testStrings)
  {
    std::cout << "Length("
              << str
              << "). Should be: "
              << str.length()
              << ". strlen returns: "
              << test::strlen(str.c_str())
              << std::endl;
    assert(test::strlen(str.c_str()) == str.length());
  }

  std::cout << "strlen: GOOD" << std::endl;
  return 0;
  
}
