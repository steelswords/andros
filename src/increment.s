.section .text
.global increment
increment:
  movl 8(%esp), %ecx
  addl $1, %ecx
  movl %ecx, %eax
  ret

