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
    
public:

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

