#ifndef _ANDROS_DEV_HWID_CPUID_HPP
#define _ANDROS_DEV_HWID_CPUID_HPP
#define USING_GCC_CPUID_IMPLEMENTATION 1

#include <stdint.h>
#include "video/ConsoleScreen.hpp"

#if USING_GCC_CPUID_IMPLEMENTATION

class CPUInformation
{
public:
  uint32_t m_maximumSupportedCPUID {0};
  uint32_t m_model {0};
  uint32_t m_signature;
  char m_vendorString[13]{'\0'};
  uint8_t m_processorCount{0};
  uint8_t m_initialLocalAPICID{0};
  bool m_apicExists {false};
  uint32_t debug;
  CPUInformation();

  void print(ConsoleScreen* screen);

};

#else 
// My sad, buggy implementation.

struct CPUIDInformation
{
  char vendorString[12 + 4] {0};
  
} __attribute__((packed));

extern "C" 
{
  int _identify_cpu(CPUIDInformation* cpuidinfo);
}

int identifyCPU(CPUIDInformation* cpuidinfo);

//extern "C" char* vendorString;

#endif
#endif
