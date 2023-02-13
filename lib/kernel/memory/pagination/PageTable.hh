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

    inline void copy(const PaginationEntryInterface *obj){
        this->entry = obj->entry;
    }
    
    inline void copyFlags(const PaginationEntryInterface *obj){
        uint64 addr = this->getAddr();
        this->copy(obj);
        this->setAddr(addr);
    }

    inline void setAddr(uint64 val){
        val &= PAGE_MASK;
        this->entry &= ~PAGE_MASK;
        this->entry |= val;
    }

    inline void setFlags(uint16 flags){
        flags &= (uint16)~PAGE_MASK;
        this->entry &= PAGE_MASK;
        this->entry |= flags;
    }

    void clearActivityFlag(){
        this->setFlag(PAGE_ACCESSED,false);
        this->setFlag(PAGE_WRITTEN_IN,false);
    }

    inline void set4MBSized(bool value){
        this->setFlag(PAGE_4MB_SIZED,value);
    }
    inline void setWriteThrough(bool value){
        this->setFlag(PAGE_WRITE_THROUGH,value);
    }
    inline void setCache(bool value){
        this->setFlag(PAGE_CACHE_DISABLED,!value);
    }

    inline void setUserSpace(bool value){
        this->setFlag(PAGE_USER,value);
    }
    inline void setSuperuserSpace(bool value){
        this->setUserSpace(!value);
    }

    inline void setPresent(bool value){
        this->setFlag(PAGE_PRESENT,value);
    }
    inline void setGlobal(bool value){
        this->setFlag(PAGE_GLOBAL,value);
    }
    inline void setWritable(bool value){
        this->setFlag(PAGE_RW,value);
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
        return (entry & PAGE_WRITTEN_IN) != 0;
    }

    bool wasAccessed(){
        return (entry & PAGE_ACCESSED) != 0;
    }

    bool isCacheDisabled(){
        return (entry & PAGE_CACHE_DISABLED) != 0;
    }

    bool isCacheEnabled(){
        return !this->isCacheDisabled();
    }

    bool isWriteBack(){
        return !this->isWriteThrough();
    }

    bool isWriteThrough(){
        return (entry & PAGE_WRITE_THROUGH) != 0;
    }

    bool isUserSpace(){
        return (entry & PAGE_USER) != 0;
    }
    
    bool isSupervisorSpace(){
        return !this->isUserSpace();
    }

    bool isWritable(){
        return (entry & PAGE_RW) != 0;
    }

    bool isReadOnly(){
        return !this->isReadOnly();
    }

    bool isPresent(){
        return (entry & PAGE_PRESENT) != 0;
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

class FrameAllocator;

class PaginationTable{
private:
    L3DirectoryTable* l4_pointer;

    PaginationEntryInterface* _getEntryLevel(uint16 l4_idx,uint16 l3_idx,uint16 l2_idx,uint16 l1_idx,PAGINATION_LEVEL level);

public:

    void setBasePointer(ptr_t l4_pointer){
        this->l4_pointer = (L3DirectoryTable*)l4_pointer;
    }

    ptr_t getBasel4_pointer(){
        return this->l4_pointer;
    }

    L3DirectoryTable* getEntryTable(uint16 pos){
        return (L3DirectoryTable*)this->getEntryLevel((uint64)pos<<39,L3);
    }

    PaginationEntryInterface* getEntryLevel(uint64 virt,PAGINATION_LEVEL level);
    PaginationEntryInterface* getOrCreateEntryLevel(FrameAllocator *frameAllocator, uint64 virt,PAGINATION_LEVEL level,uint16 flags);
    uint64 getRealAddr(uint64 virt);

    ptr_t alloc_vm(FrameAllocator *frameAllocator,uint16 base_l3,uint32 pages,uint16 flags);
    void free_vm(FrameAllocator *frameAllocator,ptr_t virtualAddr,uint16 base_l3,uint32 pages,uint16 flags);
    
    void map(FrameAllocator *frameAllocator,ptr_t virtualAddr,uint32 pages,uint16 flags);
    void unmap(FrameAllocator *frameAllocator,ptr_t virtualAddr,uint32 pages);

};

#endif