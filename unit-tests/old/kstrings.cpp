#include <iostream>
#include <cstddef>
#include <limits>
#include <assert.h>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace test
{
#define __UNIT_TEST__
#include "../src/utils/kstring.hpp"
#include "../src/utils/kstring.cpp"
}

int main()
{
  std::cout << "------------------------\n";
  std::cout << "    kstring Unit Test\n";
  std::cout << "------------------------\n";
  //Test construction.
  std::vector<std::string> testStrings = {"Primus", "Secundus", "Hello there", "General Kenobi",
    "Who ate the cat who ate the rat who lived in the house that Jack built"};

  std::map<std::string, test::kstring*> stringValues;

  std::cout << "String literals:\n----------------" << std::endl;
  for (std::string str : testStrings)
  {
    stringValues[str] = new test::kstring(str.c_str()); 
    std::cout << "Should be " << str <<": " << stringValues[str]->m_data << std::endl;
    assert(strcmp(str.c_str(), stringValues[str]->m_data) == 0);
  }

  //Test itoa functions
  std::cout << "Constructing from ints:\n----------------" << std::endl;

  std::vector<uint32_t> hexValues = {0x12345678, 0xDEADBEEF, 0xFFFFFFFF, 0xAAAABBBB,
    0xABCDEF10, 0xF100000D};

  std::map<uint32_t, test::kstring*> hexKstrings;
  for (uint32_t value : hexValues)
  {
    test::kstring* kstr = new test::kstring(value, 16);
    hexKstrings[value] = kstr;
    std::stringstream ss;
    uint64_t testValue = 0;
    testValue = std::stoul(kstr->m_data, 0, 16);
    std::cout << std::hex << "Should be " << value << ": " << testValue << std::endl;
    assert(value == testValue);
  }
  

  return 0;

}
