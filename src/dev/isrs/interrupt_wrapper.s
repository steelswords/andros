.section .text

.global addTwo
addTwo:
  mov 4(%esp), %eax
  add $1, %eax
  push %eax
  call addOne
  pop %edx
  ret
  

.global keyboardISR_wrapper
keyboardISR_wrapper:
  pusha
  /* SysV ABI requires DF to be clear on function entry */
  cld
  call keyboardHandler
  popa
  iret
