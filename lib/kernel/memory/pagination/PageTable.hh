#ifndef __PAGINATION_CLASSES_H
#define __PAGINATION_CLASSES_H

#include "memory.h"
#include "FrameManager.hh"
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
    L3,
    L2,
    L1,
    PAGE
};


class PaginationTable{
private:
    L3DirectoryTable* l4_pointer;

    PageEntry *entry;
    ptr_t offset;

    void lockEntry(){
        entry->setPresent(true);
        entry->setCache(true);
        entry->setWritable(true);
        entry->setWriteThrough(false);
        entry->setSuperuserSpace(true);
    }

    void unlockEntry(){
        entry->setPresent(false);
    }

public:

    void setVirtualSpace(ptr_t offset,ptr_t entry){
        this->offset = offset;
        this->entry = (PageEntry*)entry;
    }

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
        uint64 idx = 0;
        ptr_t addr = 0;
        PaginationEntryInterface *interface;
        switch (level)
        {
        case L3:
            idx = (virt >> (12+9*3)) & 0x1FF;
            if(this->l4_pointer==NULL)return NULL;
            addr = this->l4_pointer;
            break;
        case L2:
            idx = (virt >> (12+9*2)) & 0x1FF;
            interface = this->getEntryLevel(virt,L3);
            if(interface==NULL || !interface->isPresent())return NULL;
            addr = (ptr_t)interface->getAddr();
            break;
        case L1:
            idx = (virt >> (12+9*1)) & 0x1FF;
            interface = this->getEntryLevel(virt,L2);
            if(interface==NULL || !interface->isPresent())return NULL;
            addr = (ptr_t)interface->getAddr();
            break;
        case PAGE:
        default:
            idx = (virt >> (12)) & 0x1FF;
            interface = this->getEntryLevel(virt,L1);
            if(interface==NULL || !interface->isPresent())return NULL;
            addr = (ptr_t)interface->getAddr();
            break;
        }
        entry->setAddr((uint64)addr);
        flush_tlb_address(this->offset);
        return &((PaginationEntryInterface*)this->offset)[idx];
    }

    uint64 getRealAddr(uint64 virt){
        uint64 comp = virt & ~PAGE_MASK;
        this->lockEntry();
        PageEntry* entry = (PageEntry*)this->getEntryLevel(virt,PAGE);
        uint64 addr = (entry->isPresent())?entry->getAddr()|comp:NULL;
        this->unlockEntry();
        return addr;
    }

};

static PaginationTable kernelPaginationTable;

#endif