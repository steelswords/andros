
.section .text
.global vendorString1
.global cpuidinfo
cpuidinfo:
  .long 0xb00;
vendorString1:
  .long 12
vendorString2:
  .long 34
vendorString3:
  .long 56
vendorStringNullTermination:
  .byte 0


.global identify_cpu
identify_cpu:
  /* Load the address of the cpuid structure into cpuidinfo */
  mov $vendorString1, %eax
  mov %eax, cpuidinfo
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
  movl $0x4e6f4350, %ebx
  movl $0x55494420, %edx
  movl $0x3a272820, %ecx

load_vendor_id:
  movl %ebx, vendorString1
  movl %ecx, vendorString3
  movl %edx, vendorString2
  jmp cpuid_quit_ok

cpuid_quit_error:
  mov $1, %eax
  ret

cpuid_quit_ok:
  #mov $5, %eax
  ret

   

