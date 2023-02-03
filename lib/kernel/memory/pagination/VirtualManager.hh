#pragma once
#include "PageTable.hh"


class VirtualManager{
private:
    PaginationTable translationTable;

    uint64 next_data_addr;


public:

    VirtualManager(){
    }


};