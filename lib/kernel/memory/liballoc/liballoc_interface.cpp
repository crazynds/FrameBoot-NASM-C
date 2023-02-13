
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

void* liballoc_alloc(int pages){
    KernelController *kernel = getKernelController();
    PaginationTable* table = kernel->getKernelPaginationTable();
    return (ptr_t)table->alloc_vm(kernel->getFrameAllocator(),509,pages,PAGE_PRESENT|PAGE_RW);
}

int liballoc_free(void* ptr,int pages){
    KernelController *kernel = getKernelController();
    PaginationTable* table = kernel->getKernelPaginationTable();
    table->free_vm(kernel->getFrameAllocator(),ptr,pages);
    return 0;
}