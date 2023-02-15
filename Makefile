nasm_source_files := $(shell find asm -name *.asm)
cpp_source_files := $(shell find lib -name *.cpp)
c___source_files := $(shell find lib -name *.c)
nasm_object_files := $(patsubst asm/%.asm, bin/asm/%.o,$(nasm_source_files))
cpp_object_files := $(patsubst lib/%.cpp, bin/cpp/%.o,$(cpp_source_files))
c___object_files := $(patsubst lib/%.c, bin/c/%.o,$(c___source_files))

GCC_FLAGS=-O1 -g2 -c -m64 -fno-exceptions -ffreestanding -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -Wunused-variable -Wunused-function -I include

all: bin/os-image
	make debug
	make clean

debug: bin/os-image
	qemu-system-x86_64 -s -S bin/os-image -m 128
	make clean
	
test: bin/os-image
	qemu-system-x86_64 bin/os-image -m 128
	make clean

iso: bin/os-image
	mkdir -p iso
	cp -v bin/os-image iso/
	mkisofs -no-emul-boot -b os-image -o SO.iso iso/
	make clean

bin/os-image: bin/boot_sect.bin bin/kernel.bin
	cat bin/boot_sect.bin bin/kernel.bin > bin/os-image

$(c___object_files): bin/c/%.o : lib/%.c
	mkdir -p $(dir $@) && \
	gcc $(GCC_FLAGS) $(patsubst bin/c/%.o, lib/%.c, $@) -o $@

$(cpp_object_files): bin/cpp/%.o : lib/%.cpp
	mkdir -p $(dir $@) && \
	g++ $(GCC_FLAGS) $(patsubst bin/cpp/%.o, lib/%.cpp, $@) -o $@

$(nasm_object_files): bin/asm/%.o : asm/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst bin/asm/%.o, asm/%.asm, $@) -o $@

bin/kernel.bin: app/kernel/kernel.cpp app/app.cpp $(cpp_object_files) $(nasm_object_files) $(c___object_files)
	g++ $(GCC_FLAGS) app/kernel/kernel.cpp -o bin/kernel.o 
	g++ $(GCC_FLAGS) app/app.cpp -o bin/app.o 
	ld -e 0x7E00 -T link.ld -o bin/kernel.elf bin/app.o bin/kernel.o $(c___object_files) $(cpp_object_files) $(nasm_object_files)
	objcopy -O binary bin/kernel.elf bin/kernel.bin

bin/boot_sect.bin: boot/boot.asm bin
	nasm boot/boot.asm -o bin/boot_sect.bin

bin:
	mkdir bin

clean:
	rm -rf iso
	rm bin/* -r
