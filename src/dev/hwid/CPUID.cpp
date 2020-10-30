#include "dev/hwid/CPUID.hpp"
#include "utils/utils.hpp"
#include "stdint.h"
#include "cpuid.h"

#ifndef USING_GCC_CPUID_IMPLEMENTATION
#define USING_GCC_CPUID_IMPLEMENTATION 1
#endif

#if USING_GCC_CPUID_IMPLEMENTATION

extern "C" uint32_t byteswap32(uint32_t val);

CPUInformation::CPUInformation()
{
  unsigned int signature;
  m_maximumSupportedCPUID = __get_cpuid_max(0, &signature);
  //TODO: Check extended by calling that^ with 0x8000000 
  
  if (m_maximumSupportedCPUID == 0)
    return;

  // Get vendor
  unsigned int eax, ebx, ecx, edx;
  __get_cpuid(0, &eax, &ebx, &ecx, &edx);
  uint32_t* buffer = (uint32_t*)m_vendorString;
  buffer[0] = ebx;
  buffer[1] = ecx;
  buffer[2] = edx;
  
  //Get processor count
  __get_cpuid(1, &eax, &ebx, &ecx, &edx);
  m_processorCount = (ebx & 0xFF0000) >> 16;
  //If it's zero, than the number of processor cores is reserved.
  
  m_apicExists = (edx & 0x9);
  m_initialLocalAPICID = (uint8_t)((ebx & 0xFF000000) >> 24);
  



}

void CPUInformation::print(ConsoleScreen* screen)
{
  screen->print("Max CPUID Function: ");
  screen->printHex(m_maximumSupportedCPUID);
  screen->print("Vendor: ");
  screen->print(m_vendorString);
  screen->print(" Cores: ");
  screen->print(m_processorCount);
  screen->print(" APIC Exists: ");
  screen->print(m_apicExists);
  screen->print(" Local APIC ID: ");
  screen->print(m_initialLocalAPICID);
  screen->print(" debug = ");
  screen->print(debug);
}


#else 
int identifyCPU(CPUIDInformation* cpuidinfo)
{
  int result = _identify_cpu(cpuidinfo);
  uint32_t* vendorUint32String = (uint32_t*)cpuidinfo->vendorString;
#if 1
  vendorUint32String[0] = byteswap32(vendorUint32String[0]);
  vendorUint32String[1] = byteswap32(vendorUint32String[1]);
  vendorUint32String[2] = byteswap32(vendorUint32String[2]);
#endif
  return result;
}
#endif
