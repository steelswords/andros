cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

project(andros0.2 C CXX ASM)

# TODO
add_custom_target(iso ALL
    bash -c "${CMAKE_SOURCE_DIR}/build-helper.sh"
    BYPRODUCTS AndrOS.iso
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS andros.bin
)

# TODO: make test
add_custom_target(test
    qemu-system-x86_64 -cdrom "${CMAKE_BINARY_DIR}/AndrOS.iso"
    DEPENDS iso andros.bin
)



# Start with a huge, flat program.
add_executable(andros.bin
src/mem/MemoryManager.cpp
src/mem/memory.cpp
src/mem/GDT.cpp
src/KernelTerminal.cpp
src/dev/ps2/ps2.cpp
src/dev/keyboardScancodes.cpp
src/dev/pic/pic8259.cpp
src/dev/pic8259.cpp
src/dev/interrupts.cpp
src/dev/hwid/MultibootHeaderInfo.cpp
src/dev/hwid/CPUID.cpp
src/dev/isrs/interruptHandlers.cpp
src/video/VGATextConsoleScreen.cpp
src/KernelTerminalCommands.cpp
src/utils/kstring.cpp
src/utils/utils.cpp
src/System.cpp
src/kernel.cpp
src/System.hpp
src/mem/MemoryManager.hpp
src/mem/GDT.hpp
src/mem/memory.hpp
src/dev/ps2/ps2.hpp
src/dev/cpuIO.hpp
src/dev/keyboardScancodes.hpp
src/dev/pic/pic.hpp
src/dev/pic/pic8259.hpp
src/dev/interrupts.hpp
src/dev/hwid/MultibootHeaderInfo.hpp
src/dev/hwid/CPUID.hpp
src/dev/cpuio.hpp
src/dev/isrs/interruptHandlers.hpp
src/dev/keyboard.hpp
src/dev/x86cpu.hpp
src/dev/pic8259.hpp
src/KernelTerminal.hpp
src/video/ConsoleScreen.hpp
src/video/VGATextConsoleScreen.hpp
src/utils/kstring.hpp
src/utils/utils.hpp
src/utils/CircularBuffer.hpp
src/utils/LinkedList.hpp
src/boot.hpp
src/mem/loadGDT.s
src/dev/hwid/CPUID.s
src/dev/interruptSupport.s
src/dev/x86cpu.s
src/dev/isr.s
src/dev/keyboardISR.s
src/dev/loadIDT.s
src/dev/isrs/interrupt_wrapper.s
src/increment.s
src/boot.s
src/utils/byteswap.s
    )

target_link_options(andros.bin PUBLIC -T ${CMAKE_SOURCE_DIR}/linker.ld)

include_directories(
    src
    src/mem
    src/dev
    src/dev/hwid
    src/dev/isrs
    src/dev/pic
    src/dev/ps2
    src/utils
    src/video
)
