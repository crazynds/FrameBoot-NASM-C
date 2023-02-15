#pragma once

#include "memory/gdt/Gdt.hh"
#include "memory/pagination/FrameAllocator.hh"
#include "memory/pagination/PageTable.hh"

class Process;

class KernelController{
private:
    GDT gdt;
    FrameAllocator frameAllocator;
    PaginationTable kernelPaginationTable;
    uint64 maxMemory;
    uint16 lock_var;

    Process *currentProcess;
    
public:

    void lock();
    void unlock();

    void setCurrentProcess(Process *process){
        this->currentProcess = process;
    }
    Process* getCurrentProcess(){
        return this->currentProcess;
    }
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

