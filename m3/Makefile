kernel: kernel.o kernel_asm.o string.o
	ld86 -o kernel -d kernel.o kernel_asm.o string.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

kernel.o: kernel.c string.h
	bcc -ansi -c -o kernel.o kernel.c

string.o: string.h string.c
	bcc -ansi -c -o string.o string.c

clean:
	rm -rf kernel *.o floppya.img bochsout.txt bootload loadFile *~
