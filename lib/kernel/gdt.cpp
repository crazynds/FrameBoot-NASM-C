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


static Gdt_pointer gdt;
static Gdt_entry entries[16]; 

extern "C" void gdt_flush(Gdt_pointer *gdt);

uint32 val2=0;

uint32 addEntry(uint64 entry){
  gdt.size++;
  val2++;
  gdt.vector[gdt.size-1].entry=entry;
  return &gdt.vector[gdt.size-1]-&gdt.vector[0];
}
uint32 addEntryGDT(uint32 base, uint32 limit,uint16 acess,uint8 flags){
  uint64 entry=(limit&0x0ffff)+((base&0x00ffffff)<<16)+((uint64)acess<<40)+(((uint64)limit&0xf0000)<<32)+(((uint64)flags&0x0f)<<52)+(((uint64)base&0xff000000)<<32);
  return addEntry(entry);
}


void setupGDT(){
  gdt.size=0;
  gdt.vector=&entries[0];
  addEntry(0);
  addEntry(BIT_SEGMENTO_CODIGO|BIT_PRIVILEGIO_KERNEL|FLAG_64_BITS);
  addEntry(BIT_SEGMENTO_DADOS|BIT_PRIVILEGIO_KERNEL);
  addEntry(BIT_SEGMENTO_CODIGO|BIT_PRIVILEGIO_USER|FLAG_64_BITS);
  addEntry(BIT_SEGMENTO_DADOS|BIT_PRIVILEGIO_USER);
  gdt.size=((gdt.size)*sizeof(Gdt_entry))-1;
  gdt_flush(&gdt);
}
