.section .text

.global testInterrupts
testInterrupts:
  int $0x21 # Decimal 33
  ret

