# AndrOS

My senior design project. A chance to write some OS-level code and learn more about computer/OS architecure.

## Toolchain
AndrOS uses a GCC-based cross compiler. Here is a [pre-built one](http://newos.org/toolchains/i686-elf-4.9.1-Linux-x86_64.tar.xz) Alternately, you can build your own of a later version by following the [OSDev Wiki instructions](https://wiki.osdev.org/GCC_Cross-Compiler)

## Required Packages
- xorriso
- qemu
- qemu-system-x86
- mtools
- grub-pc-bin

## Building

To build AndrOS, run `make`. To build it and launch it in a virtual machine, type
    `make test`
