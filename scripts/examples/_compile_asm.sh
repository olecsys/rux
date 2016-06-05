#!/bin/bash
ARCHITECTURE="$(uname -m)"
BUILD_PATH="$1"
OUTPUT_FILE="$2"
MACHINE="x86"
ASM_FILE="lib/xan_linux_asm32.S"
if [ $ARCHITECTURE = "x86_64" ]
then
	MACHINE="amd64"
	ASM_FILE="lib/xan_linux_asm64.S"
elif [ $ARCHITECTURE = "i686" ]
then
	MACHINE="x86"
	ASM_FILE="lib/xan_linux_asm32.S"
fi
rm -rf "$BUILD_PATH/lib_xan_asm.o"
yasm -a x86 -f elf -m $MACHINE -p nasm -o "$BUILD_PATH/lib_xan_asm.o" $ASM_FILE
ar -rc "$OUTPUT_FILE" "$BUILD_PATH/lib_xan_asm.o"
ranlib "$OUTPUT_FILE"
