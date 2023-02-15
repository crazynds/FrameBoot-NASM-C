#include "Process.hh"
#include <kernel/class.h>
#include <kernel/alloc.h>
#include "thread.h"


static volatile uint32 __GLOBAL_UID = 0;

#include <kernel/gfx.h>
Process::Process(bool supervisor){
    KernelController *kernel = getKernelController();
    this->supervisor = supervisor;
    kernel->lock();

    // falta setar a paginação, a ideia é clonar a paginação do kernel q está no estado perfeito.
    

    this->uid = __GLOBAL_UID++;
    kernel->unlock();

    this->threads = (thread_t*)kernel_malloc(0);
    this->size_threads = 0;
    this->addThread();
}

cpu_state_t* Process::getCurrentState(){
    return &this->threads[0].state;
}

void Process::addThread(){
    this->size_threads++;
    this->threads = (thread_t*)kernel_realloc(this->threads,this->size_threads*sizeof(thread_t));
    
    // Create a stack for this thread
    // Create a Local Thread Storage for this thread
}

Process::~Process(){
    KernelController *kernel = getKernelController();
    // Liberar toda a memória consumida pelo pagination table
    uint64 addr = (uint64)this->paginatioTable.getBasel4_pointer();
    kernel->getFrameAllocator()->addMemorySpace({addr,PAGE_SIZE});


    kernel_free(this->threads);
}