# Byteswaps the 32-bit long number on the stack

.section .text
.global byteswap32

/*
 | a | b | c | d |

Keep original number in edx.
Move to eax.

and by ff and push d,
shift right by 8, and by ff, and push c
shift right by 16, and by ff, and push b
shift right by 24, and by ff. That's a in the right place.

pop b into ecx, then shift left 8  and or into eax
pop c into ecx, then shift left 16 and or into eax
pop d into ecx, then shift left 24 and or into eax

Not using loops because I'm optimizing my time here. Loops here would add debugging time.
*/

/* Also functions as a hton or ntoh. */

byteswap32:
  # Keep original number in edx
  movl 4(%esp), %edx

  # Move number to eax
  movl %edx, %eax

  # And by ff and push d
  andl $0xff, %eax
  pushl %eax

  
  # Shift right by 8, and by ff, and push c
  movl %edx, %eax
  shrl $8, %eax
  andl $0xff, %eax
  pushl %eax

  # Shift right by 16, and by ff, and push b
  movl %edx, %eax
  shrl $16, %eax
  andl $0xff, %eax
  pushl %eax

  # Shift right by 24, and by ff. That's a in the right place.
  movl %edx, %eax
  shrl $24, %eax
  andl $0xff, %eax

  # Pop b into ecx, then shift left 8  and or into eax
  popl %ecx
  shll $8, %ecx
  orl  %ecx, %eax

  # Pop c into ecx, then shift left 16 and or into eax
  popl %ecx
  shll $16, %ecx
  orl  %ecx, %eax

  # Pop d into ecx, then shift left 24 and or into eax
  popl %ecx
  shll $24, %ecx
  orl  %ecx, %eax

  ret
    

#TODO
