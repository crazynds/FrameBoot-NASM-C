#include <kernel/gfx.h>


extern const char Test[];

void main(){

  kprintStr(1,20,"USUARIO PROGRAMA",0x2F);
  kprintStr(1,21,Test,0x2F);
}