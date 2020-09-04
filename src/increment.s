.section .text
.global increment
increment:
  movl 4(%esp), %eax
  addl $1, %eax
  ret

