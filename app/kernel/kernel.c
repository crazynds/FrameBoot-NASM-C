#include <stdvar.h>
#include <kernel/gfx.h>
#include <kernel/setup.h>


void main();

void kmain(void){
  int y=2;
  clrscr();
  kprintStr(0,y++,"Kernel iniciado em 64bits        ",0x0F);
  setupPagination();
  kprintStr(0,y++,"Paginacao definida.",0x0F);
  setupGDT();
  kprintStr(0,y++,"GDT definida.",0x0F);
  return main();
}
