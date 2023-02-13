
#include <stdvar.h>
#include <kernel/gfx.h>
#include <kernel/class.h>
#include "../../KernelController.hh"
#include "../pagination/FrameAllocator.hh"

static uint16 lock = 0;

extern "C" int acquireLock(uint16*);
extern "C" void releaseLock(uint16*);

int liballoc_lock(){
    return acquireLock(&lock);
}

int liballoc_unlock(){
    releaseLock(&lock);
    return 0;
}

void* liballoc_alloc(int){
    KernelController *kernel = getKernelController();
    kprintStr(20,0,"TENTATIVA DE ALOC",BACKGROUND_BLACK|TEXT_WHITE);
    FrameAllocator* fm = kernel->getFrameAllocator();
    uint64 val = fm->allocate();
    while (true);
    
    return (ptr_t)nullptr;
}

int liballoc_free(void* ptr,int pages){
    //FrameAllocator& fm = FrameAllocator::getInstance();
    //memory_space space = {(uint64)ptr,PAGE_SIZE*pages};
    //fm.addMemorySpace(space);
    return 0;
}