OUTPUT_FORMAT("binary")

ENTRY(start)

/* Endereco fisico onde o Kernel sera alocado */
phys = 0X7E00;

/* Exposicoes ao ld */
SECTIONS
{
  .text phys : AT(phys) {
    code = .;
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
/* Endereco fisico onde os Dados serão alocados */
  .data : AT(phys + (data - code))
  {
    data = .;
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