OUTPUT_FORMAT("elf64-x86-64")

ENTRY(_start)

/* Endereco fisico onde o Kernel sera alocado */
phys = 0X7E00;
VIRTUAL_OFFSET_ADDR = 0xFFFFFF8000000000;
//VIRTUAL_OFFSET_ADDR = 0x8000000000;
//VIRTUAL_OFFSET_ADDR = 0;


VIRTUAL_KERNEL_ADDR = phys + VIRTUAL_OFFSET_ADDR;

/* Exposicoes ao ld */
SECTIONS
{
  .text VIRTUAL_KERNEL_ADDR : AT(VIRTUAL_KERNEL_ADDR) {
    code = .;
    *(.entry)
    *(.text)
    . = ALIGN(4096);
  }
  .data : AT(VIRTUAL_KERNEL_ADDR + (data - code))
  {
    data = .;
    *(.rodata)
    *(.data)
    . = ALIGN(4096);
    *(.stack)
    . = ALIGN(4096);
  }
  .bss : AT(VIRTUAL_KERNEL_ADDR + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }
  end = .;
}