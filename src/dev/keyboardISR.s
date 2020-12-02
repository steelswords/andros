.section .data

/* TODO: Make a buffer and a C++ class that wraps around it. */
.global keyboardScancode
.global keyboardBufferIndex
keyboardScancode:
  .long 0
keyboardBufferIndex:
  .long 0

.section .text

/* If irq (argument 0) >= 8, then 
   commandPort = PIC2_COMMAND
   else
    commandPort = 
sendEOIToPIC:
  movl 4(%exp), %eax
  test %eax, $8
  jg 
*/
  
.global modifyScancode
modifyScancode:
  mov keyboardScancode, %eax
  add $1, %eax
  mov %eax, keyboardScancode
  ret

.global keyboardISR
keyboardISR:
  push %eax
  xor %eax, %eax
  in $0x60, %al # Read info from the keyboard
  mov %al, keyboardScancode
  # call modifyScancode
  

/*TODO: Make this reference the function already written */
/* Acknowledge the interrupt to the PIC
  mov $0x20, %al
  out %al, $0x20
  pop %eax
  iret
