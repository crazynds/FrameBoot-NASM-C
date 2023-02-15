#include "KernelController.hh"
#include <kernel/gfx.h>
#include "process/Process.hh"

extern "C" int acquireLock(uint16*);
extern "C" void releaseLock(uint16*);

static KernelController kernelController;


KernelController *getKernelController(){
    return &kernelController;
}
void KernelController::lock(){
    acquireLock(&this->lock_var);
}
void KernelController::unlock(){
    releaseLock(&this->lock_var);
}

void panic(const char *str){
    kprintStr(0,24,str,BACKGROUND_WHITE|TEXT_RED);
    while(true);
}

//extern "C" void saveCpuState(cpu_state_t *);

void saveCurrentState(){
    //cpu_state_t *state = getKernelController()->getCurrentProcess()->getCurrentState();
    //saveCpuState(state);
}