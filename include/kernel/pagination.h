#ifndef PAGINATION_H
    #define PAGITAION_H

    #include <stdvar.h>

    #define L4 1
    #define L3 512          //all L3 in L4 map 512GB
    #define L2 262144       //all L2 in L3 map 1GB
    #define L1 134217728    //all L1 in L2 map 2MB

    #define RW 1<<1
    #define PRESENT 1<<0
    #define USER 1<<2
    #define WRITE_THROUGH 1<<3
    #define PAGE_4MB_SIZED 1<<7

    #define REAL_PAG_MEM 0x100000


    char *FAULT[8];

#endif
