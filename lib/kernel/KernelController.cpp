#include "KernelController.hh"
#include <kernel/gfx.h>

extern "C" int acquireLock(uint16*);
extern "C" void releaseLock(uint16*);

static KernelController kernelController;


KernelController *getKernelController(){
    return &kernelController;
}
void KernelController::lock(){
    acquireLock(&this->lock);
}
void KernelController::unlock(){
    releaseLock(&this->lock);
}

void panic(const char *str){
    kprintStr(0,24,str,BACKGROUND_WHITE|TEXT_RED);
    while(true);
}
