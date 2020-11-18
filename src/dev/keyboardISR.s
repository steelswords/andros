.section .data

/* TODO: Make a buffer and a C++ class that wraps around it. */
.global keyboardScancode
keyboardScancode:
  .long 0

.section .text

.global keyboardISR
keyboardISR:
  push %eax
  in $0x60, %al # Read info from the keyboard
  mov %al, keyboardScancode

/*TODO: Make this reference the function already written */
/* Acknowledge the interrupt to the PIC
  mov $0x20, %al
  out %al, $0x20
  pop %eax
  iret
