SPECSDIR=specification
BUILDDIR=build
SPECIFICATION_SOURCES = $(wildcard $(SPECSDIR)/*.tex)
AS="/opt/cross/bin/i686-elf-as"
CC="/opt/cross/bin/i686-elf-gcc"
CPP="/opt/cross/bin/i686-elf-g++"
FLAGS= -fno-exceptions -ffreestanding -fno-rtti -O2 -Wall -Wextra

all: video_sys kernel

.PHONY: dirs
dirs:
	mkdir -p $(BUILDDIR)

.PHONY: video_sys
video_sys: $(BUILDDIR)/VGATextConsoleScreen.o

VIDEO_SOURCES = $(wildcard video/*.cpp)
$(BUILDDIR)/VGATextConsoleScreen.o: $(VIDEO_SOURCES) $(wildcard video/*.hpp)
	$(CPP) -Ivideo -I. $(FLAGS) -c -o $(BUILDDIR)/VGATextConsoleScreen.o $(VIDEO_SOURCES)


kernel: boot.s vga_text_kernel.c dirs video_sys
	$(AS) boot.s -o $(BUILDDIR)/boot.o
# $(CC) -c vga_text_kernel.c -o $(BUILDDIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	$(CPP) $(FLAGS) -Ivideo -I. -c kernel.cpp -o $(BUILDDIR)/kernel.o -ffreestanding -O2 -Wall -Wextra
	
#	$(CC) -T linker.ld -o $(BUILDDIR)/andros.bin -ffreestanding -O2 -nostdlib $(BUILDDIR)/boot.o $(BUILDDIR)/kernel.o $(BUILDIR)/VGATextConsoleScreen.o
	$(CPP) -T linker.ld -o $(BUILDDIR)/andros.bin $(FLAGS) -nostdlib $(BUILDDIR)/boot.o $(BUILDDIR)/kernel.o $(BUILDDIR)/VGATextConsoleScreen.o

.PHONY: spec
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

.PHONY: test
test: $(BUILDDIR)/AndrOS.iso
	qemu-system-x86_64 -cdrom build/AndrOS.iso
