#pragma once

#include <vector>
#include "memory/gdt/Gdt.hh"
#include "memory/pagination/FrameAllocator.hh"
#include "process/Process.hh"

class KernelController{
private:
    GDT gdt;
    FrameAllocator frameAllocator;
    
public:

    FrameAllocator* getFrameAllocator(){
        return &frameAllocator;
    }

    GDT* getGDT(){
        return &gdt;
    }
};
