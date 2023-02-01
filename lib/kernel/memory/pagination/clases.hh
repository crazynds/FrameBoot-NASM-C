#ifndef __PAGINATION_CLASSES_H
#define __PAGINATION_CLASSES_H
#include <stdvar.h>

#define PAGE_SIZE 0x1000
#define PAGE_MASK (~(PAGE_SIZE-1))

#define RW (1<<1)
#define PRESENT (1<<0)
#define USER (1<<2)
#define WRITE_THROUGH (1<<3)
#define CACHE_DISABLED (1<<4)
#define ACCESSED (1<<5)
#define WRITTEN_IN (1<<5)
#define PAGE_4MB_SIZED (1<<7)

struct memory_space{
    uint64 base;
    uint64 size;
};

struct memory_map{
    memory_space space;
    uint32 type;
    uint32 extendedAtb;
}__attribute__((packed));


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

    ptr_t getAddr(){
        return (ptr_t)(entry&(PAGE_MASK));
    }

    uint64 getValue(){
        return this->entry;
    }

    void copy(const PaginationEntryInterface obj){
        this->entry = obj.entry;
    }

    void setAddr(ptr_t addr){
        uint64 val = (uint64)addr;
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

    ptr_t getRealAddr(uint64 virt){
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
        return entry->getAddr();
    }

};

#endif