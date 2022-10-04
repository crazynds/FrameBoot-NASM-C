#ifndef MEMORY_H
    #define MEMORY_H

    #include <stdvar.h>

    void* malloc(uint64 size);
    void* realloc(void* ptr,uint64 size);
    void* calloc(uint64 tam,uint32 size);
    void free(void *ptr);


#endif