#include "KernelController.hh"
#include <kernel/gfx.h>

static KernelController kernelController;

KernelController *getKernelController(){
    return &kernelController;
}

void panic(const char *str){
    kprintStr(0,24,str,BACKGROUND_WHITE|TEXT_RED);
    while(true);
}
