#pragma once

#include <stdvar.h>
#include <vector>
#include "../memory/pagination/PageTable.hh"
#include "../KernelController.hh"

typedef struct fd_interface fd_interface_t;
class Thread;

class Process{
private:
    uint32 uid;
    uint32 size_threads;
    Thread *threads;

    fd_interface_t *interface;
    PaginationTable paginatioTable;
    bool supervisor;

public:
    Process(bool supervisor);

    ~Process();

    void addThread();




    bool isSupervisor(){
        return supervisor;
    }
    bool isUser(){
        return !supervisor;
    }


};





