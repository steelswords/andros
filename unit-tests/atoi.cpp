#include <iostream>
#include <limits>
#include <cstddef>
#include <assert.h>
#include <sstream>

namespace test
{
#define __UNIT_TEST__ 1
#include "libc/stdlib.c"
}

int main()
{
  std::stringstream builder;

  std::cout << "----------------------------\n";
  std::cout << "  atoi Unit Test" << std::endl;
  std::cout << "----------------------------\n";
  std::cout << "Should be 1234567809: " << test::atoi("1234567809") << std::endl;
  std::cout << "Should be 123456789: "  << test::atoi("123456789")  << std::endl;
  std::cout << "Should be 12345678: "   << test::atoi("12345678")   << std::endl;
  std::cout << "Should be 1234567: "    << test::atoi("1234567")    << std::endl;
  std::cout << "Should be 123456: "     << test::atoi("123456")     << std::endl;
  std::cout << "Should be 12345: "      << test::atoi("12345")      << std::endl;
  std::cout << "Should be 1234: "       << test::atoi("1234")       << std::endl;
  std::cout << "Should be 123: "        << test::atoi("123")        << std::endl;
  builder.str("");
  builder << 123;
  assert(123 == test::atoi(builder.str().c_str()));
  
  std::cout << "Should be 12: "         << test::atoi("12")         << std::endl;
  std::cout << "Should be 1: "          << test::atoi("1")          << std::endl;
  std::cout << "Should be -42: "        << test::atoi("-42")        << std::endl;
  builder.str("");
  builder << -42;
  assert(-42 == test::atoi(builder.str().c_str()));

  std::cout << "Should be -001200: "        << test::atoi("-001200")        << std::endl;
  builder.str("");
  builder << -001200;
  assert(-001200 == test::atoi(builder.str().c_str()));




  //Check values
  //Minimum value
  int min = std::numeric_limits<int>::min();
  builder.str("");
  builder << min;
  std::string minString = builder.str();
  std::cout << "Should be " << minString << ": " << test::atoi(minString.c_str()) << std::endl;

  //Maximum value
  int max = std::numeric_limits<int>::max();
  builder.str("");
  builder << max;
  std::string maxString = builder.str();
  std::cout << "Should be " << maxString << ": " << test::atoi(maxString.c_str()) << std::endl;

  //Assert values
  assert(min == test::atoi(minString.c_str()));
  assert(max == test::atoi(maxString.c_str()));

  std::cout << "All good!" << std::endl;
  return 0;
}
