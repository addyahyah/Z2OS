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
bochs -f opsys.bxrc
