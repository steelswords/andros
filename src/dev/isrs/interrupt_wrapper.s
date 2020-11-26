.section .text

.global addTwo
addTwo:
  mov 4(%esp), %eax
  add $1, %eax
  push %eax
  call addOne
  pop %edx
  ret
  
