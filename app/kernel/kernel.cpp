#include <stdvar.h>
#include <kernel/gfx.h>
#include <kernel/setup.h>
#include <kernel/class.h>


void main();

extern "C" void kmain(void){
  KernelController *kernel = getKernelController();
  int y=2;
  color base = BACKGROUND_BLACK|TEXT_WHITE;
  clrscr(base);
  kprintStr(0,y++,"Kernel iniciado em 64bits",base);
  setupGDT(kernel);
  kprintStr(0,y++,"GDT definida.",base);
  setupPagination(kernel);
  kprintStr(0,y++,"Paginacao definida.",base);
  setupIDT(kernel);
  kprintStr(0,y++,"IDT definida.",base);
  return main();
}
