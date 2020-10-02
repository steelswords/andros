#ifndef _ANDROS_DEV_HWID_CPUID_HPP
#define _ANDROS_DEV_HWID_CPUID_HPP

struct CPUIDInformation
{
  char vendorString[12 + 1];
  
} __attribute__((packed));

extern "C" CPUIDInformation* cpuidinfo;

extern "C" int identify_cpu();

extern "C" int* vendorString1;


#endif
