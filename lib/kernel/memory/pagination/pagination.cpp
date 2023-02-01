#include <kernel/gfx.h>
#include "clases.hh"
#include <stdvar.h>
#include <stdlib.h>


#define MAX_BUFFER_FRAMES 16

struct list_frame_t{
    memory_space data;
    list_frame_t *next;
};

typedef struct list_frame_t list_frame_t;

list_frame_t* list_push(list_frame_t *list,memory_space data){
    if(list==nullptr){
        list = new list_frame_t;
        list->next = nullptr;
        list->data = data;
    }else if(list->data.base > data.base){
        list_frame_t *tmp = list;
        list = new list_frame_t;
        list->next = tmp;
        list->data = data;
    }
    return list;
}


class FrameManager{
private:
    uint8 qtd_buffer = 0;
    list_frame_t *avaliableMemory = nullptr;
    uint64 frame_buffer[MAX_BUFFER_FRAMES];


    bool loadBuffer(){
        while(qtd_buffer < MAX_BUFFER_FRAMES && this->avaliableMemory != nullptr){
            memory_space space = this->avaliableMemory->data;
            uint64 qtdFrames = space.size/PAGE_SIZE;

            if(qtdFrames == 0){
                list_frame_t *next = this->avaliableMemory->next;
                delete this->avaliableMemory;
                this->avaliableMemory = next;
            }else{
                this->addFrameBuffer(space.base & PAGE_MASK);
                space.base += PAGE_SIZE;
                space.size -= PAGE_SIZE;
            }
        }
        return qtd_buffer>0;
    }

public:


    ptr_t allocate(){
        if(this->qtd_buffer<=0 && !this->loadBuffer())
            return NULL;
        return (ptr_t)this->frame_buffer[--this->qtd_buffer];
    }

    void free(ptr_t val){
        uint64 frame = (uint64)val & (PAGE_MASK);
        this->addMemoryMap({
            frame, PAGE_SIZE
        });

    }

    void addFrameBuffer(uint64 frame){
        this->frame_buffer[this->qtd_buffer++] = frame;
    }

    void addMemoryMap(memory_space map){
        map.base = map.base & (~0xFFF);
        this->avaliableMemory = list_push(this->avaliableMemory,map);
    }

};


static FrameManager frameManager;
static PaginationTable kernelPaginationTable;


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

    for(int x=0;mem->space.size!=0;x++){
        
        kprintStr(1,10+x,"Memoria",0x0f);

        kprinthex(15,10+x,mem->space.base);
        kprinthex(30,10+x,mem->space.size);
        kprintnum(45,10+x,mem->type);
        kprintnum(60,10+x,mem->extendedAtb);
        if(mem->type==1 && mem->space.base>=0x100000){
            if(x==0){
                for(int y=0;y<MAX_BUFFER_FRAMES/2 && mem->space.size > 0;y++,mem->space.size-=PAGE_SIZE,mem->space.base+=PAGE_SIZE){
                    frameManager.addFrameBuffer(mem->space.base);
                }
            }
            if(mem->space.base+mem->space.size>maxMemory)maxMemory= mem->space.base+mem->space.size;
            //addMemoryEntry(mem->base,mem->size);
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

    ptr_t base = (ptr_t) (NEXT_PAGING_KERNEL + p++ * 0x1000);
    L3DirectoryTable *l3 = kernelPaginationTable.getEntryTable(0);
    L2Directory *l2 = l3->getEntryTable(0);
    L1Table *l1 = l2->getEntryTable(1);
    l1->clear();
    l1->setAddr((ptr_t)(NEXT_PAGING_KERNEL + p++ * 0x1000));
    l1->setPresent(true);
    l1->setCache(true);
    l1->setWriteThrough(true);
    l1->setWritable(true);

    uint64 addr = 0x00;

    for(uint64 x=0;x<512;x++){
        PageEntry *entry = l1->getEntryTable(x);
        entry->clear();
        entry->setAddr((ptr_t) (addr + x * 0x1000));
        entry->setWritable(true);
        entry->setPresent(true);
        entry->setCache(true);
        entry->setWriteThrough(true);
    }
    kprinthex(20,20,(uint64)kernelPaginationTable.getRealAddr(0x250000));
}
