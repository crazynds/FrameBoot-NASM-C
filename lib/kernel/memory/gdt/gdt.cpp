#include "../../KernelController.hh"
#include "Gdt.hh"


extern "C" void gdt_flush(Gdt_pointer *gdt);

void GDT::setup(){
  this->gdt.vector = (Gdt_entry*)&entries;
  this->gdt.size = 0;
  this->size = 0;
  this->addEntry(0);
}

uint32 GDT::addEntry(uint64 entry){
  gdt.vector[this->size].entry=entry;
  gdt.size = ((this->size+1)*sizeof(Gdt_entry))-1;
  return &gdt.vector[this->size++]-&gdt.vector[0];
}
uint32 GDT::addEntryGDT(uint32 base, uint32 limit,uint16 acess,uint8 flags){
  uint64 entry=(limit&0x0ffff)+((base&0x00ffffff)<<16)+((uint64)acess<<40)+(((uint64)limit&0xf0000)<<32)+(((uint64)flags&0x0f)<<52)+(((uint64)base&0xff000000)<<32);
  return addEntry(entry);
}
Gdt_pointer* GDT::getPointer(){
  return &this->gdt;
}

void setupGDT(KernelController *kernel){
  GDT *gdt = kernel->getGDT();
  gdt->setup();
  gdt->addEntry(BIT_SEGMENTO_CODIGO|BIT_PRIVILEGIO_KERNEL|FLAG_64_BITS);
  gdt->addEntry(BIT_SEGMENTO_DADOS|BIT_PRIVILEGIO_KERNEL);
  gdt->addEntry(BIT_SEGMENTO_CODIGO|BIT_PRIVILEGIO_USER|FLAG_64_BITS);
  gdt->addEntry(BIT_SEGMENTO_DADOS|BIT_PRIVILEGIO_USER);
  gdt_flush(gdt->getPointer());
}
