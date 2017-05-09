all: kernel shell

kernel: kernel.o kernel_asm.o string.o
	ld86 -o kernel -d kernel.o kernel_asm.o string.o

shell: shell.o lib_asm.o string.o
	ld86 -o shell -d shell.o string.o lib_asm.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

kernel.o: kernel.c string.h
	bcc -ansi -c -o kernel.o kernel.c

string.o: string.h string.c
	bcc -ansi -c -o string.o string.c

shell.o: shell.c
	bcc -ansi -c -o shell.o shell.c

lib_asm.o: lib.asm
	as86 lib.asm -o lib_asm.o

clean:
	rm -rf kernel shell *.o floppya.img bochsout.txt bootload loadFile stringMaker *~
