#include "Process.hh"
#include <kernel/class.h>
#include <kernel/alloc.h>

static uint32 __GLOBAL_UID = 0;


struct fd_interface{
    void (*write)(void*,uint32);
    void (*write_error)(void*,uint32);
    void* (*read)(void*,uint32);
};

Process::Process(bool supervisor){
    KernelController *kernel = getKernelController();
    this->uid = GLOBAL_UID++;
    this->supervisor = supervisor;
    uint64 addr = kernel->getFrameAllocator()->allocate();
    paginatioTable.setBasePointer((ptr_t)addr);
    kernel->lock();
    L3DirectoryTable *face = (L3DirectoryTable *)kernel->getKernelPaginationTable()->getEntryLevel(0,L3);
    face->setAddr(addr);
    face->setPresent(true);
    face->setSuperuserSpace(true);
    L3DirectoryTable *face2 = (L3DirectoryTable *)kernel->getKernelPaginationTable()->getEntryLevel(0,L2);
    face2[0].copy((PaginationEntryInterface){0});
    for(int x=1;x<512;x++){
        face2[x].copy(face[x]);
    }
    face->setPresent(false);
    kernel->unlock();
    this->threads = (Thread*)kernel_malloc(0);
    this->size_threads = 0;
    this->addThread();
}

void addThread(){
    this->size_threads++;
    this->threads = (Thread*)kernel_realloc(this->threads,this->size_threads*sizeof(Thread));
    new(&this->threads[this->size_threads-1]) Thread();
}

Process::~Process(){
    KernelController *kernel = getKernelController();
    // Liberar toda a memória consumida pelo pagination table
    uint64 addr = (uint64)this->paginatioTable.getBasel4_pointer();
    kernel->getFrameAllocator()->addMemorySpace({addr,PAGE_SIZE});


    for(int x=0;x<this->size_threads;x++){
        this->threads[x].~Thread();
    }
}