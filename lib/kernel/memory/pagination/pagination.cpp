#include <kernel/gfx.h>
#include "PageTable.hh"
#include "FrameManager.hh"
#include <stdvar.h>
#include <stdlib.h>


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
    uint64 maxMemory = 0x100000;
    FrameManager &frameManager = FrameManager::getInstance();

    for(int x=0;mem->space.size!=0;x++){
        
        kprintStr(1,10+x,"Memoria",0x0f);
        kprinthex(15,10+x,mem->space.base);
        kprinthex(30,10+x,mem->space.size);
        kprintnum(45,10+x,mem->type);
        kprintnum(60,10+x,mem->extendedAtb);
        if(mem->type==1 && mem->space.base>=0x100000){
            if(x==0){
                for(int y=0;y<MAX_BUFFER_FRAMES/2 && mem->space.size > 0;y++,mem->space.size-=PAGE_SIZE,mem->space.base+=PAGE_SIZE){
                    frameManager.addFrameInBuffer(mem->space.base);
                }
            }
            if(mem->space.base+mem->space.size>maxMemory)maxMemory= mem->space.base+mem->space.size;
            frameManager.addMemoryMap(mem->space);
        }
        mem ++;
    }
    return maxMemory&0xFFFFFFFFFFFFFE00;
}

void setupPagination(){
    uint64 NEXT_PAGING_KERNEL=0x100000;
    uint64 p = 0;
    kernelPaginationTable.setBasePointer((ptr_t) 0x2000);
    uint64 maxMemory = loadMemoryInformation();

    uint64 base = (NEXT_PAGING_KERNEL + p++ * 0x1000);
    L3DirectoryTable *l3 = kernelPaginationTable.getEntryTable(0);
    L2Directory *l2 = l3->getEntryTable(0);
    L1Table *l1 = l2->getEntryTable(1);
    l1->clear();
    l1->setAddr((NEXT_PAGING_KERNEL + p++ * 0x1000));
    l1->setPresent(true);
    l1->setCache(true);
    l1->setWriteThrough(true);
    l1->setWritable(true);

    uint64 addr = 0x00;

    for(uint64 x=0;x<512;x++){
        PageEntry *entry = l1->getEntryTable(x);
        entry->clear();
        entry->setAddr((addr + x * 0x1000));
        entry->setWritable(true);
        entry->setPresent(true);
        entry->setCache(true);
        entry->setWriteThrough(true);
    }
    kprinthex(20,20,(uint64)kernelPaginationTable.getRealAddr(0x250000));
}
