#include "dev/hwid/CPUID.hpp"
#include "utils/utils.hpp"
#include "stdint.h"


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
