.section .data

.global interruptReturnValue
interruptReturnValue:
  .long 12


.section .text
.global defaultISR
defaultISR:
  push %eax
  mov $interruptReturnValue, %eax
  add $1, %eax
  mov %eax, interruptReturnValue
  pop %eax
  iret
  
  
.global testInterrupts
testInterrupts:
  int $0x35
  ret

