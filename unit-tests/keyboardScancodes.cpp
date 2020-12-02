#include <iostream>
#include <vector>
#include <cstdint>

#include "../src/dev/keyboardScancodes.cpp"

int main()
{

  CircularBuffer<uint8_t> input(100);
  std::vector<uint8_t> inputRaw = {42, 20, 148, 170, 19, 147, 23, 151, 31, 159, 20, 148, 30, 158, 49, 177, 29, 56, 184, 157};
  //std::vector<uint8_t> inputRaw = {42, 20, 148, 170};
  CircularBuffer<char> output(100);

  // Add raw scancodes to input
  std::cout << "Raw scancode input: ";
  for (uint8_t code : inputRaw)
  {
    std::cout << "<" << (int)code << ">, ";
    input.add(code);
  }
  std::cout << std::endl;

  std::cout << "Processing input....\n";
  processKeyboardInput(&input, output);
  std::cout  << "Done." << std::endl;

  std::cout << "Output: ";
  while(!output.isEmpty())
  {
    std::cout << output.get();
  }

  std::cout << std::endl;
  
  return 0;

}
