SPECSDIR=specification
BUILDDIR=build
SPECIFICATION_SOURCES = $(wildcard $(SPECSDIR)/*.tex)

SDIRS = src/ src/video
SDIR = src
ODIR = obj
BDIR = build
DDIR = dep
LDIR = lib
ISO = $(BDIR)/AndrOS.iso
KNAME = andros.bin

INC = $(SDIRS)
INC_PARAMS = $(foreach d, $(INC), -I$d)
DEPFLAGS   = -MT $@ -MMD -MP -MF $(DDIR)/$*.Td
FLAGS= -fno-exceptions -ffreestanding -fno-rtti -O2 -Wall -Wextra

CROSS_COMPILER_DIR = "/opt/cross/bin"

AS  = $(CROSS_COMPILER_DIR)/i686-elf-as 
CC  = $(CROSS_COMPILER_DIR)/i686-elf-gcc $(FLAGS) $(DEPFLAGS) $(INC_PARAMS)
CPP = $(CROSS_COMPILER_DIR)/i686-elf-g++ $(FLAGS) $(DEPFLAGS) $(INC_PARAMS)
POSTCPPC   = @mv -f $(DDIR)/$*.Td $(DDIR)/$*.d && touch $@

# Default make target: set up the environment, build the kernel, and launch VM
# TODO: Do I really want this pipe here, making it an order-only prerequisite?
.PHONY: all
all: | toolchain $(KNAME) test

# List all sources to be included in the project (except main file)
_SRCS = $(foreach f, $(SDIRS), $(wildcard $(f)/*.cpp))

# Derived variable SRCS used by dependency management
SRCS = kernel.cpp $(_SRCS)

# Derived variables KOBJ lists dependencies for the output binaries
KOBJ =  $(patsubst src/%.cpp, $(ODIR)/%.o, $(_SRCS))

# Program link step
$(BDIR)/$(KNAME): $(KOBJ)
	$(CPP) -T linker.ld -o $@ $^

# All compilation units in the project
$(ODIR)/%.o: $(SDIR)/%.cpp $(DDIR)/%.d
	$(CPP) -c -o $@ $<
	$(POSTCPPC)

$(ODIR)/video/%.o: $(SDIR)/video/%.cpp $(DDIR)/video/%.d
	$(CPP) -c -o $@ $<
	$(POSTCPPC)

.PHONY: clean
clean:
	rm -rf $(BDIR) $(DDIR) $(ODIR)
	rm $(wildcard specification/*.aux) ||:
	rm specification/SpecTemplate.log specification/texput.log ||:



###################################
#OLD CODE
############
#.PHONY: dirs
#dirs:
#	mkdir -p $(BUILDDIR)
#
#.PHONY: video_sys
#video_sys: $(BUILDDIR)/VGATextConsoleScreen.o
#
#VIDEO_SOURCES = $(wildcard video/*.cpp)
#$(BUILDDIR)/VGATextConsoleScreen.o: $(VIDEO_SOURCES) $(wildcard video/*.hpp)
#	$(CPP) -Ivideo -I. $(FLAGS) -c -o $(BUILDDIR)/VGATextConsoleScreen.o $(VIDEO_SOURCES)
#
#
#kernel: boot.s vga_text_kernel.c dirs video_sys
#	$(AS) boot.s -o $(BUILDDIR)/boot.o
## $(CC) -c vga_text_kernel.c -o $(BUILDDIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
#	$(CPP) $(FLAGS) -Ivideo -I. -c kernel.cpp -o $(BUILDDIR)/kernel.o -ffreestanding -O2 -Wall -Wextra
#	
##	$(CC) -T linker.ld -o $(BUILDDIR)/andros.bin -ffreestanding -O2 -nostdlib $(BUILDDIR)/boot.o $(BUILDDIR)/kernel.o $(BUILDIR)/VGATextConsoleScreen.o
#	$(CPP) -T linker.ld -o $(BUILDDIR)/andros.bin $(FLAGS) -nostdlib $(BUILDDIR)/boot.o $(BUILDDIR)/kernel.o $(BUILDDIR)/VGATextConsoleScreen.o
#
#.PHONY: spec
#spec: $(SPECSDIR)/SpecTemplate.pdf
#
##This target quits with an error, but it still works.
#$(SPECSDIR)/SpecTemplate.pdf: $(SPECIFICATION_SOURCES) 
#	cd $(SPECSDIR) ; pdflatex --shell-escape SpecTemplate.tex
#	clean
#
#.PHONY: clean
#clean:
#
#	rm specification/$(wildcard *.aux)
#	rm specification/SpecTemplate.log specification/texput.log
#	rm -rf $(BUILDDIR)
#
#.PHONY: iso
#iso: $(BUILDDIR)/AndrOS.iso



#######################################################
# MAKEOVER
#######################################################

# Setup of directory structures
.PHONY: toolchain kmain $(KNAME) test
toolchain: | $(BDIR) $(ODIR) $(DDIR) $(ISO) 
kmain: $(BDIR)/$(KNAME)
$(KNAME): | toolchain kmain


# List of subdirectories we have to make
OBJ_SUBDIRS = $(patsubst src/%, $(ODIR)/%, $(SDIRS))
BLD_SUBDIRS = $(patsubst src/%, $(BDIR)/%, $(SDIRS))

$(DDIR):
	@mkdir $(DDIR)

$(ODIR):
	@mkdir $(ODIR)
	$(foreach dir, $(OBJ_SUBDIRS), @mkdir -p $(dir))
	

$(BDIR):
	@mkdir $(BDIR)
	$(foreach dir, $(BLD_SUBDIRS), @mkdir -p $(dir))

.ONESHELL:
$(ISO): $(KMAIN)
	cd $(BUILDDIR)
	mkdir -p iso/boot/grub
	cp andros.bin iso/boot
	cp ../grub.cfg iso/boot/grub
	pwd
	grub-mkrescue -o AndrOS.iso iso

.PHONY: test
test: $(ISO)
	qemu-system-x86_64 -cdrom $(BDIR)/AndrOS.iso




#Dependency management: prevent .d files from being deleted.
$(DDIR)/%.d: ;
.PRECIOUS: $(DDIR)/%.d

include $(wildcard $(patsubst %,$(DDIR)/%.d,$(basename $(SRCS))))
