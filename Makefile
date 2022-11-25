SPECSDIR=specification
BUILDDIR=build
SPECIFICATION_SOURCES = $(wildcard $(SPECSDIR)/*.tex)

SDIRS = src/ src/video src/dev src/mem src/libc src/libc/include \
				src/dev/hwid src/utils src/dev/pic src/dev/ps2 src/dev/isrs
SDIR = src
ODIR = obj
BDIR = $(BUILDDIR)
DDIR = dep
LDIR = lib
ISRDIR = src/dev/isrs
ISO = $(BDIR)/AndrOS.iso
KNAME = andros.bin

#INC = $(SDIRS)
INC        = $(patsubst src/%.hpp, %.hpp, $(SDIRS)) $(patsubst src/%.h, %.h, $(SDIRS))
INC_PARAMS = $(foreach d, $(INC), -I$d)
DEPFLAGS   = -MT $@ -MMD -MP -MF $(DDIR)/$*.Td
FLAGS= -fno-exceptions -ffreestanding -O2 -Wall -Wextra -nostdlib -mno-red-zone
CPPFLAGS = $(FLAGS) -fno-rtti -std=c++11
#TODO: Investigate why this is throwing an error
#FLAGS_ISR= -mgeneral-regs-only
FLAGS_ISR = -Wall

#CROSS_COMPILER_DIR = "/opt/cross/bin"
CROSS_COMPILER_DIR = "/home/tristan/opt/cross/bin"

AS  = $(CROSS_COMPILER_DIR)/i686-elf-as 
CC  = $(CROSS_COMPILER_DIR)/i686-elf-gcc -std=c99 $(FLAGS) $(INC_PARAMS)
CPP = $(CROSS_COMPILER_DIR)/i686-elf-g++ $(CPPFLAGS) $(DEPFLAGS) $(INC_PARAMS)
CPPL= $(CROSS_COMPILER_DIR)/i686-elf-g++ $(FLAGS)
POSTCPPC   = @mv -f $(DDIR)/$*.Td $(DDIR)/$*.d && touch $@

# Default make target: set up the environment, build the kernel, and launch VM
# TODO: Do I really want this pipe here, making it an order-only prerequisite?
.PHONY: all
#all: | toolchain $(KNAME) test
all: | toolchain kmain 

# List all sources to be included in the project (except main file)
_SRCS    = $(foreach f, $(SDIRS), $(wildcard $(f)/*.cpp))
## ISR_SRCS = $(foreach f, $(ISRDIR), $(wildcard $(f)/*.cpp))
## ISR_OBJ  = $(patsubst src/%.cpp, $(ODIR)/%.isro, $(ISR_SRCS))
ASM_SRCS = $(foreach f, $(SDIRS) $(ISRDIR), $(wildcard $(f)/*.s))
ASM_OBJ  = $(patsubst src/%.s, $(ODIR)/%._o, $(ASM_SRCS))
C_SRCS   = $(foreach f, $(SDIRS), $(wildcard $(f)/*.c))
C_OBJ    = $(patsubst src/%.c, $(ODIR)/%.co, $(C_SRCS))

# Derived variable SRCS used by dependency management
SRCS = $(_SRCS) $(C_SRCS) 
# $(ISR_SRCS)

# Derived variables KOBJ lists dependencies for the output binaries
#KOBJ =  $(foreach dir, $(SDIRS), $(patsubst $(dir)/%.cpp, $(ODIR)/%.o, $(_SRCS)))
KOBJ =  $(patsubst src/%.cpp, $(ODIR)/%.o, $(SRCS))

#####################
# Kernel link step
#####################
$(BDIR)/$(KNAME): $(KOBJ) $(ASM_OBJ) $(C_OBJ) $(ISR_OBJ)
	echo -e "--> Compiling Kernel"
# $(AS) src/boot.s -o $(ODIR)/boot.o
# $(CPP) -T linker.ld -o $@ $^ $(ODIR)/boot.o
	echo -e "--> Linking kernel"
	$(CPP) -T linker.ld -o $@ $^

# All c++ compilation units in the project
$(ODIR)/%.o: $(SDIR)/%.cpp $(DDIR)/%.d
	echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
	$(CPP) -c -o $@ $<
	$(POSTCPPC)

# All asm compilation units in the project
$(ODIR)/%._o: $(SDIR)/%.s $(DDIR)/%.d
	echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Assembling:\033[0m" $<
	$(AS) -c -o $@ $<
	#$(POSTCPPC)

# All C compilation units in the project
$(ODIR)/%.co: $(SDIR)/%.c $(DDIR)/%.d
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
	$(CC) -c -o $@ $<
	#$(POSTCPPC)

## # All the ISR (Interrupt Service Routines) units in the project
## $(ODIR)/%.isro: $(ISR_SRCS)/%.cpp $(DDIR)/%.d
## 	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
## 	# Redo the interrupt_wrapper module
## 	$(AS) -c -o $(SDIR)/$(ISRDIR)/interrupt_wrapper._o
## 	$(CPP) $(FLAGS_ISR) -c -o $@ $< $(SDIR)/$(ISRDIR)/interrupt_wrapper._o
## 	#$(POSTCPPC)
	

$(ODIR)/video/%.o: $(SDIR)/video/%.cpp $(DDIR)/video/%.d
	@echo -e "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $<
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

OBJ_SUBDIRS = $(patsubst src/%, $(ODIR)/%, $(SDIRS) $(ISRDIR))
BLD_SUBDIRS = $(patsubst src/%, $(BDIR)/%, $(SDIRS) $(ISRDIR))
DEP_SUBDIRS = $(patsubst src/%, $(DDIR)/%, $(SDIRS) $(ISRDIR))

$(DDIR):
	@mkdir $(DDIR)
	$(foreach dir, $(DEP_SUBDIRS), @mkdir -p $(dir))

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
	grub-mkrescue -v -o AndrOS.iso iso

.PHONY: test
test: $(ISO)
	qemu-system-x86_64 -cdrom $(BDIR)/AndrOS.iso

.PHONY: debug
debug: $(ISO)
	qemu-system-x86_64 -cdrom $(BDIR)/AndrOS.iso -d int,in_asm -no-reboot

#Dependency management: prevent .d files from being deleted.
$(DDIR)/%.d: ;
.PRECIOUS: $(DDIR)/%.d

include $(wildcard $(patsubst %,$(DDIR)/%.d,$(basename $(SRCS))))
