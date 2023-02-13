#include "PageTable.hh"
#include "FrameAllocator.hh"
#define CONVERT_PAGE_IDX_TO_ADDR(L4_IDX,L3_IDX,L2_IDX,L1_IDX) (((((((((L4_IDX>=256?((uint64)~0&(~((uint64)(1<<9)-1))):0))|L4_IDX)<<9)|L3_IDX)<<9|L2_IDX)<<9)|L1_IDX)<<12)


PaginationEntryInterface* PaginationTable::getOrCreateEntryLevel(FrameAllocator *frameAllocator, uint64 virt,PAGINATION_LEVEL level,uint16 flags){
    uint16 l1_idx = (virt>>12)&0x1FF;
    uint16 l2_idx = (virt>>21)&0x1FF;
    uint16 l3_idx = (virt>>30)&0x1FF;
    uint16 l4_idx = (virt>>39)&0x1FF;
    uint64 virtualAddrArr[4] = {
        CONVERT_PAGE_IDX_TO_ADDR(510,l4_idx,l3_idx,l2_idx) + l1_idx*8,
        CONVERT_PAGE_IDX_TO_ADDR(510,510,l4_idx,l3_idx) + l2_idx*8,
        CONVERT_PAGE_IDX_TO_ADDR(510,510,510,l4_idx) + l3_idx*8,
        CONVERT_PAGE_IDX_TO_ADDR(510,510,510,510) + l4_idx*8,
    };
    PaginationEntryInterface* resp=NULL;
    bool present = true;
    for(int loop_level = L3; loop_level >= level; loop_level-=1){
        resp = (PaginationEntryInterface*)virtualAddrArr[loop_level];
        if(!resp->isPresent() || !present){
            resp->setAddr(frameAllocator->allocate());
            resp->setFlags(flags);
            resp->setPresent(true);
            present = false;
        }
    }
    return resp;
}

inline PaginationEntryInterface* PaginationTable::_getEntryLevel(uint16 l4_idx,uint16 l3_idx,uint16 l2_idx,uint16 l1_idx,PAGINATION_LEVEL level){
    uint64 virtualAddrArr[4] = {
        CONVERT_PAGE_IDX_TO_ADDR(510,l4_idx,l3_idx,l2_idx) + l1_idx*8,
        CONVERT_PAGE_IDX_TO_ADDR(510,510,l4_idx,l3_idx) + l2_idx*8,
        CONVERT_PAGE_IDX_TO_ADDR(510,510,510,l4_idx) + l3_idx*8,
        CONVERT_PAGE_IDX_TO_ADDR(510,510,510,510) + l4_idx*8,
    };
    PaginationEntryInterface* resp=NULL;
    for(int loop_level = L3; loop_level >= level; loop_level-=1){
        resp = (PaginationEntryInterface*)virtualAddrArr[loop_level];
        if(!resp->isPresent())
            return NULL;
    }
    return resp;
}

PaginationEntryInterface* PaginationTable::getEntryLevel(uint64 virt,PAGINATION_LEVEL level){
    uint16 l1_idx = (virt>>12)&0x1FF;
    uint16 l2_idx = (virt>>21)&0x1FF;
    uint16 l3_idx = (virt>>30)&0x1FF;
    uint16 l4_idx = (virt>>39)&0x1FF;
    return this->_getEntryLevel(l4_idx,l3_idx,l2_idx,l1_idx,level);
}

uint64 PaginationTable::getRealAddr(uint64 virt){
    uint64 comp = virt & ~PAGE_MASK;
    PageEntry* entry = (PageEntry*)this->getEntryLevel(virt,PAGE);
    uint64 addr = (entry->isPresent())?entry->getAddr()|comp:NULL;
    return addr;
}

ptr_t PaginationTable::alloc_vm(FrameAllocator *frameAllocator,uint16 base_l3,uint32 pages,uint16 flags){
    uint16 x=0,y=0,z=0;
    while(z<512 || (z==512 && y<512)){
        PageEntry *entry = (PageEntry*)this->_getEntryLevel(base_l3,z,y,x,PAGE);
        uint32 i;
        for(i=0;entry==NULL && i<pages;i++){
            entry = (PageEntry*)this->_getEntryLevel(base_l3,z+(y + (x+i)/512)/512,(y + (x+i)/512)%512,(x + i)%512,PAGE);
        }
        if(entry==NULL){
            // Encontrei um lugar 
            ptr_t addr = (ptr_t)CONVERT_PAGE_IDX_TO_ADDR(base_l3,z,y,x);
            this->map(frameAllocator,addr,pages,flags);
            return addr;
        }

        x += i + 1;
        if(x>512){
            x=0;
            y++;
            if(y>512){
                y=0;
                z++;
            }
        }
    }
}
void PaginationTable::free_vm(FrameAllocator *frameAllocator,ptr_t virtualAddr,uint16 base_l3,uint32 pages,uint16 flags){

}

void PaginationTable::map(FrameAllocator *frameAllocator,ptr_t virtualAddr,uint32 pages,uint16 flags){
    uint64 addr = (uint64)virtualAddr;
    for(uint32 x=0;x<pages;x++,addr += PAGE_SIZE){
        PageEntry *page = (PageEntry*)this->getOrCreateEntryLevel(frameAllocator,addr,PAGE,flags);
    }
}
void PaginationTable::unmap(FrameAllocator *frameAllocator,ptr_t virtualAddr,uint32 pages){
    uint32 x=0;
    bool erased = false;
    uint64 virAddr = (uint64)virtualAddr;
    for(uint16 l4_idx = (virAddr>>39)&0x1FF; l4_idx < 512 && x<pages; l4_idx++){
        for(uint16 l3_idx = (virAddr>>30)&0x1FF; l3_idx < 512 && x<pages; l3_idx++){
            for(uint16 l2_idx = (virAddr>>21)&0x1FF; l2_idx < 512 && x<pages; l2_idx++){
                for(uint16 l1_idx = (virAddr>>12)&0x1FF; l1_idx < 512 && x<pages; l1_idx++, x++){
                    PageEntry *page= (PageEntry*)this->_getEntryLevel(l4_idx,l3_idx,l2_idx,l1_idx,PAGE);
                    if(page==NULL)continue;
                    uint64 addr = page->getAddr();
                    page->setPresent(false);
                    frameAllocator->addMemorySpace({addr,PAGE_SIZE});
                    erased = true;
                }
                if(erased){
                    PageEntry* pages= (PageEntry*)this->_getEntryLevel(l4_idx,l3_idx,l2_idx,0,PAGE);
                    for(int y=0;y<512;y++){
                        if(pages[y].isPresent()){
                            erased = false;
                            break;
                        }
                    }
                    if(erased){
                        L1Table *table = (L1Table *)this->_getEntryLevel(l4_idx,l3_idx,l2_idx,0,L1);
                        uint64 addr = table->getAddr();
                        table->setPresent(false);
                        frameAllocator->addMemorySpace({addr,PAGE_SIZE});
                    }
                }
            }
            if(erased){
                L1Table *tables= (L1Table *)this->_getEntryLevel(l4_idx,l3_idx,0,0,L1);
                for(int y=0;y<512;y++){
                    if(tables[y].isPresent()){
                        erased = false;
                        break;
                    }
                }
                if(erased){
                    L2Directory *directory = (L2Directory *)this->_getEntryLevel(l4_idx,l3_idx,0,0,L2);
                    uint64 addr = directory->getAddr();
                    directory->setPresent(false);
                    frameAllocator->addMemorySpace({addr,PAGE_SIZE});
                }
            }
        }
        if(erased){
            L2Directory *directories= (L2Directory *)this->_getEntryLevel(l4_idx,0,0,0,L2);
            for(int y=0;y<512;y++){
                if(directories[y].isPresent()){
                    erased = false;
                    break;
                }
            }
            if(erased){
                L3DirectoryTable *dirTable = (L3DirectoryTable *)this->_getEntryLevel(l4_idx,0,0,0,L3);
                uint64 addr = dirTable->getAddr();
                dirTable->setPresent(false);
                frameAllocator->addMemorySpace({addr,PAGE_SIZE});
            }
        }
    }

}