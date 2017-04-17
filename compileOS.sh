nasm bootload.asm
dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
make clean
make
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
bochs -f opsys.bxrc
