asm_source_files := $(shell find asm -name *.asm)
asm_object_files := $(patsubst asm/%.asm, bin/asm/%.o,$(asm_source_files))

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


bin/all_src.c: lib/* compile.sh
	./compile.sh

$(asm_object_files): bin/asm/%.o : asm/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst bin/asm/%.o, asm/%.asm, $@) -o $@

bin/kernel.bin: app/kernel/kernel.c app/app.c bin/all_src.c $(asm_object_files)
	gcc -O2 -c app/kernel/kernel.c -o bin/kernel.o -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function -I include
	gcc -O2 -c app/app.c -o bin/app.o -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function -I include
	gcc -O2 -c bin/all_src.c -o bin/compiled.o -std=gnu99 -m64 -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -Wincompatible-pointer-types -nodefaultlibs -Wunused-variable -Wunused-function -I include
	ld -e 0x7E00 -T link.ld -o bin/kernel.bin bin/app.o bin/kernel.o bin/compiled.o $(asm_object_files)

bin/boot_sect.bin: boot/boot.asm bin
	nasm boot/boot.asm -o bin/boot_sect.bin

bin:
	mkdir bin

clean:
	rm -rf iso
	rm bin/* -r
