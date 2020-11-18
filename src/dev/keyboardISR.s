.section .data

/* TODO: Make a buffer and a C++ class that wraps around it. */
.global keyboardScancode
.global keyboardBufferIndex
keyboardScancode:
  .long 0
keyboardBufferIndex:
  .long 0

.section .text

.global keyboardISR
keyboardISR:
  push %eax
  in $0x60, %al # Read info from the keyboard
  mov $0x1, %al
  mov %al, keyboardScancode
  mov $keyboardBufferIndex, %eax
  add $1, %eax
  mov %eax, keyboardBufferIndex
  

/*TODO: Make this reference the function already written */
/* Acknowledge the interrupt to the PIC
  mov $0x20, %al
  out %al, $0x20
  pop %eax
  iret
