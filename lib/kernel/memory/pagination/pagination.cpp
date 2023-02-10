#include <kernel/gfx.h>
#include <stdvar.h>
#include <kernel/address.h>

#include "PageTable.hh"
#include "FrameManager.hh"

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
}


uint64 loadMemoryInformation(){
    struct memory_map *mem = (struct memory_map*)(0x7E000+24);
    uint64 maxMemory = 0;
    FrameManager &frameManager = FrameManager::getInstance();

    for(int x=0;mem->space.size!=0;x++){
        kprintStr(1,10+x,"Memoria",0x0f);
        kprinthex(15,10+x,mem->space.base);
        kprinthex(30,10+x,mem->space.size);
        kprintnum(45,10+x,mem->type);
        kprintnum(60,10+x,mem->extendedAtb);
        if(mem->type==1 && mem->space.base>=0x100000){
            maxMemory+=mem->space.size&PAGE_MASK;
            frameManager.addMemorySpace(mem->space);
        }
        mem ++;
    }
    return maxMemory;
}

void prepareKernelPaginationTable(){
    L3DirectoryTable *l3 = (L3DirectoryTable*) L4_ADDR;
    L2Directory *l2 = (L2Directory*) l3[511].getEntryTable(511);
    l2->setAddr(0x5000);
    l2->setPresent(true);
    l2->setCache(false);
    l2->setSuperuserSpace(true);
    l2->setWritable(true);
    l2->setWriteThrough(true);
    L1Table *l1 = (L1Table*) l2->getEntryTable(0);
    l1->setAddr(0x6000);
    l1->setPresent(true);
    l1->setCache(false);
    l1->setSuperuserSpace(true);
    l1->setWritable(true);
    l1->setWriteThrough(true);
    kernelPaginationTable.setBasePointer((ptr_t) L4_ADDR);
    kernelPaginationTable.setVirtualSpace((ptr_t)PAGINATION_TEMP_OFFSET,(ptr_t)(KERNEL_OFFSET|0x6000));
}


void setupPagination(){
    uint64 NEXT_PAGING_KERNEL=0x100000;
    uint64 p = 0;
    prepareKernelPaginationTable();
    //uint64 maxMemory = loadMemoryInformation();

    uint64 base = (NEXT_PAGING_KERNEL + p++ * 0x1000);
    L3DirectoryTable *l3 = kernelPaginationTable.getEntryTable(0);
    l3->setPresent(false);
    uint64 realAddr = kernelPaginationTable.getRealAddr((uint64)setupPagination);
    //kprinthex(20,20,realAddr);
    //kprintnum(20,20,15);
    while(true);
    kprinthex(20,19,(uint64)VGA_ADDRESS);
    while(true);
    kprinthex(20,20,kernelPaginationTable.getRealAddr((uint64)setupPagination));
    kprinthex(20,21,(uint64)setupPagination);
    kprinthex(20,22,(uint64)&NEXT_PAGING_KERNEL);
    while(true);

}
