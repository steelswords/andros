.section .bss
.global isrReturnValue
isrReturnValue:
  .long 0

.section .text

.global defaultInterruptServiceRoutine
defaultInterruptServiceRoutine:
  mov $3, %eax
  mov %eax, isrReturnValue
  ret


.global  isr_wrapper
isr_wrapper:
  pushal
  cld
  call defaultInterruptServiceRoutine
  popal
  iret

