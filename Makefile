
kernel: kernel.o kernel_asm.o
	ld86 -o kernel -d kernel.o kernel_asm.o
kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

kernel.o: kernel.c
	bcc -ansi -c -o kernel.o kernel.c

clean:
	rm -rf kernel kernel.o kernel_asm.o *~

