#include <stdvar.h>
#include <kernel/asmFunctions.h>
#include <kernel/gfx.h>


void main();

void kmain(void){
  int y=2;
  clrscr();
  kprintStr(0,y++,"Kernel iniciado em 64bits        ",0x0F);  
  
  return main();
}
