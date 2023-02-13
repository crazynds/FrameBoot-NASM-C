#pragma once

#include <stdvar.h>


#define MAX_BUFFER_FRAMES 16
#define PAGE_SIZE ((uint64)0x1000)
#define PAGE_MASK (~(PAGE_SIZE-1))

#define PAGE_RW (1<<1)
#define PAGE_PRESENT (1<<0)
#define PAGE_USER (1<<2)
#define PAGE_WRITE_THROUGH (1<<3)
#define PAGE_CACHE_DISABLED (1<<4)
#define PAGE_ACCESSED (1<<5)
#define PAGE_WRITTEN_IN (1<<5)
#define PAGE_4MB_SIZED (1<<7)
#define PAGE_GLOBAL (1<<8)

struct memory_space{
    uint64 base;
    uint64 size;
};

struct memory_map{
    memory_space space;
    uint32 type;
    uint32 extendedAtb;
}__attribute__((packed));



