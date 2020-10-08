.section bss
gdtr: 
  .word 0 # size
  .long 0 # address

.section .text
.global _loadGDT

_loadGDT:
  cli
  movl 4(%esp), %eax
  movl $gdtr, %edx
  movl %eax, 2(%edx)
  movw 6(%esp), %ax
  movw %ax, (%edx)
  lgdt (%edx)
  sti
  ret

  
