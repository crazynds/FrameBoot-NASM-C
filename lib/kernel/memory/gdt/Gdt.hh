#pragma once

#include <stdvar.h>


#define BIT_ESCRITA ((uint64)1<<41)
#define BIT_EXECUTAVEL ((uint64)1<<43)
#define BIT_PRESENTE ((uint64)1<<47)
#define BIT_GRANULARIDADE ((uint64)1<<55)
#define BIT_PRIVILEGIO_KERNEL 0
#define BIT_PRIVILEGIO_USER ((uint64)3<<45)
#define BIT_SEGMENTO_DADOS ((uint64)1<<44)|BIT_PRESENTE|BIT_ESCRITA|BIT_GRANULARIDADE
#define BIT_SEGMENTO_CODIGO ((uint64)1<<44)|BIT_PRESENTE|BIT_EXECUTAVEL|BIT_GRANULARIDADE

#define FLAG_64_BITS ((uint64)1<<53)


typedef struct{
  uint64 entry;
}Gdt_entry;

typedef struct{
  uint16 size;
  Gdt_entry *vector;
}__attribute__((packed)) Gdt_pointer;


class GDT{
private:
  Gdt_pointer gdt;
  Gdt_entry entries[16];
  uint8 size;


public:

  void setup();

  uint32 addEntry(uint64 entry);
  uint32 addEntryGDT(uint32 base, uint32 limit,uint16 acess,uint8 flags);
  Gdt_pointer* getPointer();

};
