.section .text


.global defaultInterruptServiceRoutine
defaultInterruptServiceRoutine:
  ret


.global  isr_wrapper
isr_wrapper:
  pushal
  cld
  call defaultInterruptServiceRoutine
  popal
  iret

