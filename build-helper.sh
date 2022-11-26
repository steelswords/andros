#!/bin/bash
# This script makes a bootable .iso file for the emulator from the kernel.
# It is called during 'make iso' and 'make test'.
# There is a way to condense this to just CMake, but I didn't want to invest the time today.

set -ue -o pipefail
echo "Going to make the .iso file"
currentDirBasename=$(basename "$(pwd)" )
if ! [[ "$currentDirBasename" =~ "build" ]];
then 
    echo "Error: Need to be in build directory (Currently in $(pwd)"
    exit
fi
mkdir -p iso/boot/grub
cp ../grub.cfg iso/boot/grub
cp andros.bin iso/boot
grub-mkrescue -o AndrOS.iso iso
echo "Done making iso"
