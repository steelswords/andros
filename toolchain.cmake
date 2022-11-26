# Documentation: https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
# the name of the target operating system
set(CMAKE_SYSTEM_NAME Generic)

# which compilers to use for C and C++
set(CROSS_DIR /home/tristan/opt/cross)
set(CMAKE_C_COMPILER   ${CROSS_DIR}/bin/i686-elf-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_DIR}/bin/i686-elf-g++)
set(CMAKE_ASM_COMPILER ${CROSS_DIR}/bin/i686-elf-as)
set(CMAKE_AR           ${CROSS_DIR}/bin/i686-elf-ar)
set(CMAKE_LINKER       ${CROSS_DIR}/bin/i686-elf-ld)
set(CMAKE_OBJCOPY      ${CROSS_DIR}/bin/i686-elf-objcopy)
set(CMAKE_RANLIB       ${CROSS_DIR}/bin/i686-elf-ranlib)

set(FLAGS "-fno-exceptions -ffreestanding -O2 -Wall -Wextra -nostdlib -mno-red-zone")
set(CMAKE_C_FLAGS ${FLAGS})
set(CMAKE_CXX_FLAGS "${FLAGS} -fno-rtti -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS ${FLAGS})

set(CMAKE_SYSTEM_NAME AndrOS)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  ${CROSS_DIR})

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
