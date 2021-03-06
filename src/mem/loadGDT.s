.section bss
gdtr: 
  .word 0 # size
  .long 0 # address

.section .text
.global _loadGDT

_loadGDT:
  cli
  movl 4(%esp), %eax
  lgdt (%eax)
  ljmp $0x08, $loadSegmentRegisters

loadSegmentRegisters:
  mov $0x10, %eax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs
  mov %ax, %ss
  sti
  ret
