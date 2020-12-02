#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <assert.h>
#include <cstdlib> // For rand()

#define __UNIT_TEST__
#include "../src/utils/CircularBuffer.hpp"

void testLinearRetrieval()
{
  std::vector<char> stuff {'T', 'h', 'a', 'n', 'k', 's'};
  
  CircularBuffer<char>* circleBuffer = new CircularBuffer<char>(stuff.size() + 1);
  circleBuffer->m_defaultValue = 't';

  std::cout << "Adding to circular buffer: ";
  for (char c : stuff)
  {
    if (!circleBuffer->add(c))
      std::cout << "ERROR adding " << c;
    std::cout << c << ", ";
  }
  std::cout << "." << std::endl;
  std::cout << "Now retrieving from the circle buffer" << std::endl;

  for (int i = 0; i < stuff.size(); ++i)
  {
    std::cout << (char) circleBuffer->get() << ", ";
  }
  std::cout << std::endl;
}

void testInterleavedRetrieval()
{
  std::string msg = "Is this the real life?";
  std::string msg2 = "Is this just fantasy? Caught in a landslide";
  std::shared_ptr<CircularBuffer<char>> circleBuffer(new CircularBuffer<char>(msg.length()));
  
  // Add msg to circular buffer
  std::cout << "Adding the following to circular Buffer: ";
  for (char c : msg)
  {
    assert(circleBuffer->add(c) == true);
    std::cout << c << ", ";
  }
  std::cout << std::endl;

  //Remove 1-5 bytes at a time and add as many bytes.
  for (int i = 0; i < msg2.length(); )
  {
    int numOfBytesToConsume = rand() % 5 + 1;
    int j = numOfBytesToConsume;
    std::cout << "Recevied the following " << j << " bytes: ";
    while ( j > 0 )
    {
      char gottenCharacter = circleBuffer->get();
      std::cout << gottenCharacter << ", ";
      j--;
    }
    std::cout << "Adding the following: ";
    j = numOfBytesToConsume;
    while ( j > 0 && (i < msg2.length()) )
    {
      std::cout << msg2[i] << ", ";
      assert(circleBuffer->add(msg2[i]) == true);
      i++;
      j--;
    }
    std::cout << std::endl;
  }
}

int main()
{
  
  testLinearRetrieval();
  testInterleavedRetrieval();
  return 0; 
}
