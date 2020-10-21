.section .text
.global _loadIDT

_loadIDT:
  movl 4(%esp), %eax
  lidt (%eax)
  ret 
