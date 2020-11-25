.section .data

.global interruptReturnValue
interruptReturnValue:
  .long 12

.section .text
.global defaultISR
defaultISR:
  push %eax
  mov $222, %eax
  mov %eax, interruptReturnValue
  pop %eax
  iret
  
  
  
.global testInterrupts
testInterrupts:
  int $0x50
  ret

.set RFLAGS_IF_BIT, 0x200
.global enableInterruptFlag
enableInterruptFlag:
  sti
  ret
  pushf
  xor %eax, %eax
  pop %eax
  or %eax, RFLAGS_IF_BIT
  push %eax
  popf
  ret
