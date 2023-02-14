#include <kernel/gfx.h>
#include <stdvar.h>
#include <kernel/address.h>
#include "../../KernelController.hh"

#include "PageTable.hh"
#include "FrameAllocator.hh"

const char* FAULT[8] = {
    "Supervisory process tried to read a non-present page entry",
    "Supervisory process tried to read a page and caused a protection fault",
    "Supervisory process tried to write to a non-present page entry",
    "Supervisory process tried to write a page and caused a protection fault",
    "User process tried to read a non-present page entry",
    "User process tried to read a page and caused a protection fault",
    "User process tried to write to a non-present page entry",
    "User process tried to write a page and caused a protection fault",
};

extern "C" void pageFaultHandler(uint32 error,uint64 endereco){
    if(error&8){
        kprintStr(0,24,"Erro RSVD" ,0x4f);
    }else if(error&16){
        kprintStr(0,24,"Erro I/D" ,0x4f);
    }else kprintStr(0,24,FAULT[error&7],0x4f);
    kprintnum(0,23,endereco);
    while(true);
}


uint64 loadMemoryInformation(KernelController *kernel){
    struct memory_map *mem = (struct memory_map*)(KERNEL_OFFSET+0x7E000+24);
    uint64 maxMemory = 0;
    FrameAllocator* frameAllocator = kernel->getFrameAllocator();

    for(int x=0;mem->space.size!=0;x++){
        kprintStr(1,10+x,"Memoria",0x0f);
        kprinthex(15,10+x,mem->space.base);
        kprinthex(30,10+x,mem->space.size);
        kprintnum(45,10+x,mem->type);
        kprintnum(60,10+x,mem->extendedAtb);
        if(mem->type==1 && mem->space.base>=0x100000){
            maxMemory+=mem->space.size&PAGE_MASK;
            frameAllocator->addMemorySpace(mem->space);
        }
        mem ++;
    }
    return maxMemory;
}

void prepareKernelPaginationTable(PaginationTable *kernelPaginationTable){
    L3DirectoryTable *l3 = &((L3DirectoryTable*) L4_ADDR)[510];
    l3->setAddr(L4_ADDR);
    l3->setSuperuserSpace(true);
    l3->setCache(true);
    l3->setWritable(true);
    l3->setPresent(true);
    l3->setWriteThrough(true);
    l3 = &((L3DirectoryTable*) L4_ADDR)[511];
    l3->setGlobal(true);
    l3->setSuperuserSpace(true);
    l3->setCache(true);
    l3->setWriteThrough(true);
    
    // Disable pagination on first's addresses
    l3 = kernelPaginationTable->getEntryTable(0);
    l3->setPresent(false);
}

#include <stdio.h>
void setupPagination(KernelController *kernel){
    PaginationTable *kernelPaginationTable = kernel->getKernelPaginationTable();
    prepareKernelPaginationTable(kernelPaginationTable);
    uint64 maxMemory = loadMemoryInformation(kernel);
    kernel->setMaxMemory(maxMemory);

    kprinthex(20,20,(uint64)kernel);  
    kprinthex(20,21,kernelPaginationTable->getRealAddr((uint64)kernel));    

    
    char buffer[100];
    uint64 free = kernel->getFreeMemory();
    uint64 max = kernel->getMaxMemory();
    int x=0;
    char arr[4] = {' ','K','M','G'};
    while(free>(1024*1024) && max>(1024*1024)){
        free/=1024;
        max/=1024;
        x++;
    }

    sprintf(buffer,"Memoria: %x %cB/%x %cB (%.2f%%)",free,arr[x],max,arr[x],((free*100.0)/max));
    kprintStr(0,23,buffer,BACKGROUND_BLACK|TEXT_WHITE);
}
