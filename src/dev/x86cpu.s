.section .text

.global getEFlags
.global setEFlags

getEFlags:
  pushf
  pop %eax
  ret

