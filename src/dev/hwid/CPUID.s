
.section .bss
.global vendorStringPtr
vendorStringPtr: .long 0
vendorStringData: .asciz "Not Yet Run."

.section .text

#.include "src/utils/byteswap.inc"

.global identify_cpu
identify_cpu:

  /* Load the address of the CPUIDInformation structure we're loading into. */
  push %edi
  movl $vendorStringData, %edi
  movl %edi, vendorStringPtr
  movl 8(%esp), %edi # edi = &cpuidinfo
  jmp cpuid_not_supported

  /* First we want to check if cpuid is supported. */
  pushfl /* Save EFlags */
  pop %eax
  movl %eax, %ebx
  xorl %eax, 0x00200000
  push %eax
  popfl
  /* See if bit 21 has changed */
  pushfl
  pop %eax
  cmp %eax, %ebx
  jz cpuid_not_supported
  mov $0, %eax
  cpuid
  jnz load_vendor_id 

cpuid_not_supported:
  movl $0x4e6f4350, %eax
  movl $0x55494420, %edx
  movl $0x3a272820, %ecx
/*
  movl $0x54545454, %eax
  movl $0x54545454, %edx
  movl $0x54545454, %ecx
  */

load_vendor_id:
  #push %edx
  #call byteswap32
  movl %eax, (%edi)
  movl %edx, 4(%edi)
  movl %ecx, 8(%edi)
  movl $0, %eax
  #movl %eax, 9(%edi) # Null terminating character
  jmp cpuid_quit_ok

cpuid_quit_error:
  mov $-1, %eax
  jmp cpuid_quit

cpuid_quit_ok:
  mov $0, %eax
  mov %edi, %eax
  jmp cpuid_quit

cpuid_quit:
  pop %edi
  ret
  

