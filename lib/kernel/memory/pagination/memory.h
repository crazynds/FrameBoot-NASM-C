#pragma once

#include <stdvar.h>


#define MAX_BUFFER_FRAMES 16
#define PAGE_SIZE 0x1000
#define PAGE_MASK (~(PAGE_SIZE-1))

#define RW (1<<1)
#define PRESENT (1<<0)
#define USER (1<<2)
#define WRITE_THROUGH (1<<3)
#define CACHE_DISABLED (1<<4)
#define ACCESSED (1<<5)
#define WRITTEN_IN (1<<5)
#define PAGE_4MB_SIZED (1<<7)
#define GLOBAL (1<<8)

struct memory_space{
    uint64 base;
    uint64 size;
};

struct memory_map{
    memory_space space;
    uint32 type;
    uint32 extendedAtb;
}__attribute__((packed));