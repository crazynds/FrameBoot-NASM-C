#ifndef __PAGINATION_CLASSES_H
#define __PAGINATION_CLASSES_H

#include "memory.h"
#include "FrameAllocator.hh"
#include <kernel/gfx.h>

extern "C" void flush_tlb_address(ptr_t addr);


class PaginationEntryInterface{
private:
    volatile uint64 entry;

protected:

    inline void setFlag(uint64 flag,bool seted){
        if(seted){
            entry |= flag;
        }else{
            uint64 mask = (uint64)~0;
            mask ^= flag;
            entry &= mask;
        }
    }

public:

    PaginationEntryInterface(uint64 val){
        this->entry = val;
    }

    uint64 getAddr(){
        return (entry&(PAGE_MASK));
    }

    uint64 getValue(){
        return this->entry;
    }

    void copy(const PaginationEntryInterface obj){
        this->entry = obj.entry;
    }

    inline void setAddr(uint64 val){
        val &= PAGE_MASK;
        this->entry &= 0xfff;
        this->entry |= val;
    }

    void clearActivityFlag(){
        this->setFlag(ACCESSED,false);
        this->setFlag(WRITTEN_IN,false);
    }

    inline void set4MBSized(bool value){
        this->setFlag(PAGE_4MB_SIZED,value);
    }
    inline void setWriteThrough(bool value){
        this->setFlag(WRITE_THROUGH,value);
    }
    inline void setCache(bool value){
        this->setFlag(CACHE_DISABLED,!value);
    }

    inline void setUserSpace(bool value){
        this->setFlag(USER,value);
    }
    inline void setSuperuserSpace(bool value){
        this->setUserSpace(!value);
    }

    inline void setPresent(bool value){
        this->setFlag(PRESENT,value);
    }
    inline void setGlobal(bool value){
        this->setFlag(GLOBAL,value);
    }
    inline void setWritable(bool value){
        this->setFlag(RW,value);
    }

    void clear(){
        entry = 0;
    }

    bool is4KBSized(){
        return !this->is4MBSized();
    }

    bool is4MBSized(){
        return (entry & PAGE_4MB_SIZED) != 0;
    }

    bool wasWrittenIn(){
        return (entry & WRITTEN_IN) != 0;
    }

    bool wasAccessed(){
        return (entry & ACCESSED) != 0;
    }

    bool isCacheDisabled(){
        return (entry & CACHE_DISABLED) != 0;
    }

    bool isCacheEnabled(){
        return !this->isCacheDisabled();
    }

    bool isWriteBack(){
        return !this->isWriteThrough();
    }

    bool isWriteThrough(){
        return (entry & WRITE_THROUGH) != 0;
    }

    bool isUserSpace(){
        return (entry & USER) != 0;
    }
    
    bool isSupervisorSpace(){
        return !this->isUserSpace();
    }

    bool isWritable(){
        return (entry & RW) != 0;
    }

    bool isReadOnly(){
        return !this->isReadOnly();
    }

    bool isPresent(){
        return (entry & PRESENT) != 0;
    }
};

class PageEntry: public PaginationEntryInterface{
};

class L1Table: public PaginationEntryInterface{
public:
    PageEntry* getEntryTable(uint16 pos){
        return &((PageEntry*)this->getAddr())[pos];
    }
};
class L2Directory: public PaginationEntryInterface{
public:
    L1Table* getEntryTable(uint16 pos){
        return &((L1Table*)this->getAddr())[pos];
    }
};
class L3DirectoryTable: public PaginationEntryInterface{
public:
    L2Directory* getEntryTable(uint16 pos){
        return &((L2Directory*)this->getAddr())[pos];
    }
};

enum PAGINATION_LEVEL{
    PAGE,
    L1,
    L2,
    L3,
};


class PaginationTable{
private:
    L3DirectoryTable* l4_pointer;

public:

    void setBasePointer(ptr_t l4_pointer){
        this->l4_pointer = (L3DirectoryTable*)l4_pointer;
    }

    ptr_t getBasel4_pointer(){
        return this->l4_pointer;
    }

    L3DirectoryTable* getEntryTable(uint16 pos){
        return &l4_pointer[pos];
    }
    PaginationEntryInterface* getEntryLevel(uint64 virt,PAGINATION_LEVEL level){
        uint16 l1_idx = (virt>>12)&0x1FF;
        uint16 l2_idx = (virt>>21)&0x1FF;
        uint16 l3_idx = (virt>>30)&0x1FF;
        uint16 l4_idx = (virt>>39)&0x1FF;
        uint16 arr[4] = {l4_idx,l3_idx,l2_idx,l1_idx};
        PaginationEntryInterface* resp=NULL;
        for(int loop_level = L3; loop_level >= level; loop_level-=1){
            uint16 *ptr_arr = (uint16*)&arr;
            uint64 virtualAddr = ((uint64)~0)&(~((uint64)(1<<9) - 1)) | 510;
            uint8 x;
            for(x=0;x<3;x++){
                virtualAddr <<= 9;
                if(x<loop_level){
                    virtualAddr|=510;
                }else{
                    virtualAddr|=*ptr_arr;
                    ptr_arr++;
                }
            }
            virtualAddr<<=12;
            PaginationEntryInterface *ptr = (PaginationEntryInterface*)virtualAddr;
            resp = &ptr[*ptr_arr];
            if(!resp->isPresent())
                return NULL;
        }
        return resp;
    }

    uint64 getRealAddr(uint64 virt){
        uint64 comp = virt & ~PAGE_MASK;
        PageEntry* entry = (PageEntry*)this->getEntryLevel(virt,PAGE);
        uint64 addr = (entry->isPresent())?entry->getAddr()|comp:NULL;
        return addr;
    }

};

#endif