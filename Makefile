SPECSDIR=specification
BUILDDIR=build
SPECIFICATION_SOURCES = $(wildcard $(SPECSDIR)/*.tex)

SDIRS = src/ src/video
SDIR = src
ODIR = obj
BDIR = $(BUILDDIR)
DDIR = dep
LDIR = lib
ISO = $(BDIR)/AndrOS.iso
KNAME = andros.bin

#INC = $(SDIRS)
INC        = $(patsubst src/%.hpp, %.hpp, $(SDIRS))
INC_PARAMS = $(foreach d, $(INC), -I$d)
DEPFLAGS   = -MT $@ -MMD -MP -MF $(DDIR)/$*.Td
FLAGS= -fno-exceptions -ffreestanding -fno-rtti -O2 -Wall -Wextra -nostdlib

CROSS_COMPILER_DIR = "/opt/cross/bin"

AS  = $(CROSS_COMPILER_DIR)/i686-elf-as 
CC  = $(CROSS_COMPILER_DIR)/i686-elf-gcc $(FLAGS) $(DEPFLAGS) $(INC_PARAMS)
CPP = $(CROSS_COMPILER_DIR)/i686-elf-g++ $(FLAGS) $(DEPFLAGS) $(INC_PARAMS)
CPPL= $(CROSS_COMPILER_DIR)/i686-elf-g++ $(FLAGS)
POSTCPPC   = @mv -f $(DDIR)/$*.Td $(DDIR)/$*.d && touch $@

# Default make target: set up the environment, build the kernel, and launch VM
# TODO: Do I really want this pipe here, making it an order-only prerequisite?
.PHONY: all
#all: | toolchain $(KNAME) test
all: | toolchain kmain 

# List all sources to be included in the project (except main file)
_SRCS = $(foreach f, $(SDIRS), $(wildcard $(f)/*.cpp))
#_ASM_SRCS = $(foreach f, $(SDIRS), $(wildcard $(f)/*.s))

# Derived variable SRCS used by dependency management
#SRCS = kernel.cpp $(_SRCS)
SRCS = kernel.cpp $(_SRCS)

# Derived variables KOBJ lists dependencies for the output binaries
#KOBJ =  $(foreach dir, $(SDIRS), $(patsubst $(dir)/%.cpp, $(ODIR)/%.o, $(_SRCS)))
KOBJ =  $(patsubst src/%.cpp, $(ODIR)/%.o, $(SRCS))

#####################
# Kernel link step
#####################
$(BDIR)/$(KNAME): $(KOBJ) 
	$(AS) src/boot.s -o $(ODIR)/boot.o
	$(CPP) -T linker.ld -o $@ $^ $(ODIR)/boot.o

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

# Setup of directory structures
.PHONY: toolchain kmain $(KNAME) test
toolchain: | $(BDIR) $(ODIR) $(DDIR) 
kmain: toolchain $(BDIR)/$(KNAME) 
#?$(KNAME): | toolchain kmain


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

.PHONY: iso
iso: kmain $(ISO)

.ONESHELL:
$(ISO): kmain
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
