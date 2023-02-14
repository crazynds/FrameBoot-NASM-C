#pragma once

#include <vector>
#include "memory/gdt/Gdt.hh"
#include "memory/pagination/FrameAllocator.hh"
#include "memory/pagination/PageTable.hh"
#include "process/Process.hh"


class KernelController{
private:
    GDT gdt;
    FrameAllocator frameAllocator;
    PaginationTable kernelPaginationTable;
    uint64 maxMemory;
    uint16 lock;
    
public:

    void lock();
    void unlock();

    void setMaxMemory(uint64 maxMemory){
        this->maxMemory = maxMemory;
    }
    uint64 getMaxMemory(){
        return this->maxMemory;
    }
    uint64 getFreeMemory(){
        return this->frameAllocator.freeMemory();
    }

    PaginationTable *getKernelPaginationTable(){
        return &kernelPaginationTable;
    }

    FrameAllocator* getFrameAllocator(){
        return &frameAllocator;
    }

    GDT* getGDT(){
        return &gdt;
    }
};

