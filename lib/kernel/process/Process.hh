#pragma once

#include <stdvar.h>
#include "../memory/pagination/PageTable.hh"
#include "../KernelController.hh"
#include "thread.h"

typedef struct fd_interface{
    void (*write)(void*,uint32);
    void (*write_error)(void*,uint32);
    void* (*read)(void*,uint32);
} fd_interface_t;



class Process{
private:
    uint32 uid;
    uint32 size_threads;
    thread_t *threads;

    fd_interface_t *interface;
    PaginationTable paginatioTable;
    bool supervisor;

public:
    Process(bool supervisor);

    ~Process();

//    uint8 addFileDescriptor(fd_interface_t fileDescriptor);

    void addThread();

    cpu_state_t* getCurrentState();




    bool isSupervisor(){
        return supervisor;
    }
    bool isUser(){
        return !supervisor;
    }


};





