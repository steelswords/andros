#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

namespace test
{
#define __UNIT_TEST__
#include "../src/utils/kstring.hpp"
#include "../src/utils/kstring.cpp"
}
uint8_t interpretHexDigit(char a)
{
  if (a <= '9' && a >= '0')
  {
    return a - 0x30;
  }
  else if (a <= 'f' && a >= 'a')
  {
    return a - 0x61 + 10;
  }
  return 0;
}

int testinterpretHexDigit()
{
  std::cout << "The numbers 0x0 through 0xf: ";
  char hexDigitsAsChars[] = {'0', '1', '2', '3', '4', '5', '6', 
    '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  for (int i = 0; i < 0x10; ++i)
  {
    char a = hexDigitsAsChars[i];
    std::cout << std::hex << a << " = " << (int)test::interpretHexDigit(a) << std::endl;
    std::cout << a << " + 1 = " << (int)test::interpretHexDigit(a) + 1 << std::endl;
    //TODO: Assert these
  }


}

void testtoIntFromHex()
{
  using namespace test;
  std::vector<std::string> testValues = {"0x1", "0x32af", "deadbeef", "b400h",
  "0x00000007", "12ab32sde"};
  for (std::string s : testValues)
  {
    size_t length = 0;
    std::cout << s << " should equal " << std::hex 
      << test::kstring::toIntFromHex(s.c_str(), length) << std::endl;
  }
  
}

int main()
{
  //testinterpretHexDigit();
  testtoIntFromHex();
  return 0;
}
