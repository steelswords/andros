/* Taken from the Bare Bones tutorial at osdev.org */
/* Constants for Multiboot header */
.set ALIGN,   1 << 0 /* Align loaded modules on page boundaries */
.set MEMINFO, 1 << 1 /* Provide memory map */
.set FLAGS,   ALIGN | MEMINFO /* The Multiboot 'flag' field */
.set MAGIC,   0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS) /* That's kind of a cheezy checksum, if you ask me. */

/* The Multiboot header that marks this program as a kernel */
.section  .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
The multiboot standard does not define the value of the stack pointer register
(esp) and it is up to the kernel to provide a stack. This allocates room for a
small stack by creating a symbol at the bottom of it, then allocating 16384
bytes for it, and finally creating a symbol at the top. The stack grows
downwards on x86. The stack is in its own section so it can be marked nobits,
which means the kernel file is smaller because it does not contain an
uninitialized stack. The stack on x86 must be 16-byte aligned according to the
System V ABI standard and de-facto extensions. The compiler will assume the
stack is properly aligned and failure to align the stack will result in
undefined behavior.
*/
.section .bss
.global stack_ptr
stack_ptr:
  .long 0

.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.align 4
kstring_area_begin:
.skip 16384 # 16 KiB. I figure 24 char * 500 strings = 12 KB. Round up.
kstring_area_end:

/* The linker designates _start to be the entry point for the kernel.
The bootloader jumps to this position when the kernel is loaded. */

.section .text
.global multiboot_header_ptr
multiboot_header_ptr:
  .long 0

.global kstring_area_begin_ptr
.global kstring_area_end_ptr
kstring_area_end_ptr:
  .long 0
kstring_area_begin_ptr:
  .long 0


.global _start
.type _start, @function
_start:
  /* At this point, the bootloader has put us in 32-bit protected mode
     Interrupts and paging are disabled. This is all documented in the
     Multiboot standard. The kernel has full control.

    ###                     ######  ####### ####### #######
     #       ##   #    #    #     # #     # #     #    #
     #      #  #  ##  ##    #     # #     # #     #    #
     #     #    # # ## #    ######  #     # #     #    #
     #     ###### #    #    #   #   #     # #     #    #
     #     #    # #    #    #    #  #     # #     #    #
    ###    #    # #    #    #     # ####### #######    #
  */

  /* Set up the stack. Remember it grows downwards (i.e. esp is decremented 
  when things are popped) */
  mov $stack_top, %esp
  mov %esp, stack_ptr

  /* Set the location of the multiboot info header */
  movl %ebx, multiboot_header_ptr

  movl $kstring_area_end, %ebx
  movl %ebx, kstring_area_end_ptr
  movl $kstring_area_begin, %ebx
  movl %ebx, kstring_area_begin_ptr

  /* TODO: Initialize the processor more, e.g. floats, load GDT, enable
           paging, etc. */

  /* Enter the high-level kernel. The ABI requires the stack is 16-byte
  aligned at the time of the call instruction. So far, nothing has been
  pushed, so 16-byte alignement is still intact. */

  call kernel_main

  /*Drop-off infinite loop: Disables interupts with cli, waits for the 
  next interrupt with hlt, and then keep jumping back to hlt if an
  interrupt comes through, like a non-maskable interrupt. */
    cli
1:  hlt
    jmp 1b

/* Set the size of the _start symbol to the current location '.' minus its
start. This is useful for debugging, apparently. */

.size _start, . - _start
