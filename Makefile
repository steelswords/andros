SPECSDIR=specification
BUILDDIR=build
SPECIFICATION_SOURCES = $(wildcard $(SPECSDIR)/*.tex)
AS="/opt/cross/bin/i686-elf-as"
CC="/opt/cross/bin/i686-elf-gcc"

all: kernel

.PHONY: dirs
dirs:
	mkdir -p $(BUILDDIR)

kernel: boot.s vga_text_kernel.c dirs 
	$(AS) boot.s -o $(BUILDDIR)/boot.o
	$(CC) -c vga_text_kernel.c -o $(BUILDDIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	$(CC) -T linker.ld -o $(BUILDDIR)/andros.bin -ffreestanding -O2 -nostdlib $(BUILDDIR)/boot.o $(BUILDDIR)/kernel.o

.PHONY: spec
#spec: SPECIFICATION_SOURCES
spec: $(SPECSDIR)/SpecTemplate.pdf

#This target quits with an error, but it still works.
$(SPECSDIR)/SpecTemplate.pdf: $(SPECIFICATION_SOURCES) 
	cd $(SPECSDIR) ; pdflatex --shell-escape SpecTemplate.tex
	clean

.PHONY: clean
clean:
	rm $(wildcard *.aux)
	rm SpecTemplate.log texput.log

.PHONY: iso
iso: $(BUILDDIR)/AndrOS.iso

.ONESHELL:
$(BUILDDIR)/AndrOS.iso: kernel
	cd $(BUILDDIR)
	mkdir -p iso/boot/grub
	cp andros.bin iso/boot
	cp ../grub.cfg iso/boot/grub
	pwd
	grub-mkrescue -o AndrOS.iso iso
