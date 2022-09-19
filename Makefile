all: bin/os-image iso
	make test
	make clean

test: bin/os-image
	qemu-system-x86_64 bin/os-image -m 128

iso: bin/os-image
	mkdir -p iso
	cp -v bin/os-image iso/
	mkisofs -no-emul-boot -b os-image -o SO.iso iso/
	make clean

bin/os-image: bin/boot_sect.bin bin/kernel.bin
	cat bin/boot_sect.bin bin/kernel.bin > bin/os-image

bin/kernel.bin: src_c/kernel.c src_c/compiled.c src_asm/kernel_entry.asm
	nasm src_asm/kernel_entry.asm -f elf64 -o bin/entry.o
	gcc -O2 -c src_c/kernel.c -o bin/kernel.o -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function
	gcc -O2 -c src_c/compiled.c -o bin/compiled.o -std=gnu99 -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function
	ld -e 0x7E00 -T src/link.ld -o bin/kernel.bin bin/entry.o bin/kernel.o bin/compiled.o

bin/boot_sect.bin: boot/boot.asm bin
	nasm boot/boot.asm -o bin/boot_sect.bin

bin:
	mkdir bin

clean:
	rm -rf iso
	rm bin/* -r
