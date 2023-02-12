#pragma once

#include <stdvar.h>
#include <vector>
#include "Thread.hh"
#include "../memory/pagination/PageTable.hh"

class Process{
private:
    uint64 uid;
    std::vector<Thread> threads;
    PaginationTable paginatioTable;

public:


};





