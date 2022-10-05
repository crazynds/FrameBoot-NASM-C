OUTPUT_FORMAT("binary")

ENTRY(start)

/* Endereco fisico onde o Kernel sera alocado */
phys = 0X7E00;

/* Exposicoes ao ld */
SECTIONS
{
  .text phys : AT(phys) {
    code = .;
    *(.entry)
    *(.text)
    . = ALIGN(4096);
  }
/* Endereco fisico onde os Dados serão alocados */
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.rodata)
    *(.data)
    . = ALIGN(4096);
  }
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }
  end = .;
}