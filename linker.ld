/* Defines the entry point that gets called after the bootloader */
ENTRY(_start)

/* Tell where various sections of the object files will be put in the
 * final kernel image. */
SECTIONS
{
  /* Begin putting sections att 1 MiB, the conventional location for 
   * bootloaders to load kernels. */
  . = 1M;

  /* First, put the multiboot header, as it is required to be put within the
   * first 8 KiB of the file on a 4-byte alignment.
   * Then, include the .text section.
   */
  .text BLOCK(64K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }
  
  /* Read-only data */
  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  /* Read-write data (initialized) */
  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  /* Read-write data (uninitialized) and stack */
  .bss BLOCK(64K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }

  /* The compiler may produce other sections, by default it will put
   * them in a segment with the same name. Simply add stuff here as needed. */
}
