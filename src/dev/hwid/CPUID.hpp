#ifndef _ANDROS_DEV_HWID_CPUID_HPP
#define _ANDROS_DEV_HWID_CPUID_HPP

#include <stdint.h>

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
