#ifndef __PAGINATION_CLASSES_H
#define __PAGINATION_CLASSES_H

#include "memory.h"
#include "FrameManager.hh"

class PaginationEntryInterface{
private:
    uint64 entry;

protected:

    void setFlag(uint64 flag,bool seted){
        if(seted){
            entry |= flag;
        }else{
            uint64 mask = (uint64)~0;
            mask ^= flag;
            entry &= mask;
        }
    }

public:

    uint64 getAddr(){
        return (entry&(PAGE_MASK));
    }

    uint64 getValue(){
        return this->entry;
    }

    void copy(const PaginationEntryInterface obj){
        this->entry = obj.entry;
    }

    void setAddr(uint64 val){
        val &= PAGE_MASK;
        this->entry &= 0xfff;
        this->entry |= val;
    }

    void clearActivityFlag(){
        this->setFlag(ACCESSED,false);
        this->setFlag(WRITTEN_IN,false);
    }

    void set4MBSized(bool value){
        this->setFlag(PAGE_4MB_SIZED,value);
    }
    void setWriteThrough(bool value){
        this->setFlag(WRITE_THROUGH,value);
    }
    void setCache(bool value){
        this->setFlag(CACHE_DISABLED,!value);
    }

    void setUserSpace(bool value){
        this->setFlag(USER,value);
    }
    void setSuperuserSpace(bool value){
        this->setUserSpace(!value);
    }

    void setPresent(bool value){
        this->setFlag(PRESENT,value);
    }
    void setWritable(bool value){
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


class PaginationTable{
private:
    L3DirectoryTable* pointer;
public:

    void setBasePointer(ptr_t pointer){
        this->pointer = (L3DirectoryTable*)pointer;
    }

    ptr_t getBasePointer(){
        return this->pointer;
    }

    L3DirectoryTable* getEntryTable(uint16 pos){
        return &pointer[pos];
    }

    void unmap(uint64 virtualAddr,uint64 qtdPages){
        virtualAddr >>= 12;
        uint16 l1_idx = virtualAddr & 0x1FF;
        virtualAddr >>= 9;
        uint16 l2_idx = virtualAddr & 0x1FF;
        virtualAddr >>= 9;
        uint16 l3_idx = virtualAddr & 0x1FF;
        virtualAddr >>= 9;
        uint16 l4_idx = virtualAddr & 0x1FF;
        for(uint64 count = 0;l4_idx < 512 && count < qtdPages; l4_idx++){
            L3DirectoryTable* l3 = this->getEntryTable(l4_idx);
            for(;l3_idx < 512 && count < qtdPages; l3_idx++){
                L2Directory* l2 = l3->getEntryTable(l3_idx);
                for(;l2_idx < 512 && count < qtdPages; l2_idx++){
                    L1Table* l1 = l2->getEntryTable(l2_idx);
                    for(;l1_idx < 512 && count < qtdPages; l1_idx++){
                        PageEntry* entry = l1->getEntryTable(l1_idx);

                    }
                }
            }
        }
    }

    void map(uint64 virtualAddr,uint64 realAddr, uint64 qtdPages){
        virtualAddr >>= 12;
        uint16 l1_idx = virtualAddr & 0x1FF;
        virtualAddr >>= 9;
        uint16 l2_idx = virtualAddr & 0x1FF;
        virtualAddr >>= 9;
        uint16 l3_idx = virtualAddr & 0x1FF;
        virtualAddr >>= 9;
        uint16 l4_idx = virtualAddr & 0x1FF;
        FrameManager &fm = FrameManager::getInstance();
        
        

        for(uint64 count = 0;l4_idx < 512 && count < qtdPages; l4_idx++){
            L3DirectoryTable* l3 = this->getEntryTable(l4_idx);
            if(!l3->isPresent()){
                l3->setAddr(fm.allocate());
                l3->setPresent(true);
                l3->setCache(true);
                l3->setSuperuserSpace(true);
                l3->setWritable(true);
                l3->setWriteThrough(false);
            }
            for(;l3_idx < 512 && count < qtdPages; l3_idx++){
                L2Directory* l2 = l3->getEntryTable(l3_idx);
                if(!l2->isPresent()){
                    l2->setAddr(fm.allocate());
                    l2->setPresent(true);
                    l2->setCache(true);
                    l2->setSuperuserSpace(true);
                    l2->setWritable(true);
                    l2->setWriteThrough(false);
                }
                for(;l2_idx < 512 && count < qtdPages; l2_idx++){
                    L1Table* l1 = l2->getEntryTable(l2_idx);
                    if(!l1->isPresent()){
                        l1->setAddr(fm.allocate());
                        l1->setPresent(true);
                        l1->setCache(true);
                        l1->setSuperuserSpace(true);
                        l1->setWritable(true);
                        l1->setWriteThrough(false);
                    }
                    for(;l1_idx < 512 && count < qtdPages; l1_idx++){
                        PageEntry* entry = l1->getEntryTable(l1_idx);
                        entry->setPresent(true);
                        entry->setCache(true);
                        entry->setSuperuserSpace(true);
                        entry->setWritable(true);
                        entry->setWriteThrough(false);
                        entry->setAddr(realAddr);
                        realAddr+=PAGE_SIZE;
                        qtdPages++;
                    }
                }
            }
        }

    }

    uint64 getRealAddr(uint64 virt){
        uint64 comp = virt & (~PAGE_MASK);
        virt >>= 12;
        uint16 l1_idx = virt & 0x1FF;
        virt >>= 9;
        uint16 l2_idx = virt & 0x1FF;
        virt >>= 9;
        uint16 l3_idx = virt & 0x1FF;
        virt >>= 9;
        uint16 l4_idx = virt & 0x1FF;
        
        L3DirectoryTable* l3 = this->getEntryTable(l4_idx);
        L2Directory* l2 = l3->getEntryTable(l3_idx);
        L1Table* l1 = l2->getEntryTable(l2_idx);
        PageEntry *entry = l1->getEntryTable(l1_idx);
        return entry->getAddr() | comp;
    }

};

static PaginationTable kernelPaginationTable;

#endif