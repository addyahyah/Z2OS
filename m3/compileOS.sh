#!/usr/bin/env bash
BASEDIR=$(dirname "$0")
cd $BASEDIR
make clean
cp ./test.img ./floppya.img
nasm bootload.asm
dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
make
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
gcc -o loadFile loadFile.c
./loadFile message.txt
bochs -f opsys.bxrc
