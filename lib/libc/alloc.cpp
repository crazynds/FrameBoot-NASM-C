#include <kernel/panic.h>
#include <kernel/alloc.h>
#include <stdlib.h>

void* malloc(size_t size){
    void* ptr = kernel_malloc(size);
    if(ptr==NULL)panic("[MALLOC - FALTA DE MEMORIA]");
    return ptr;
}
void* calloc(size_t qObj, size_t size){
    void* ptr = kernel_calloc(qObj,size);
    if(ptr==NULL)panic("[CALLOC - FALTA DE MEMORIA]");
    return ptr;
}
void* realloc(void* ptr, size_t size){
    ptr = kernel_realloc(ptr,size);
    if(ptr==NULL)panic("[REALLOC - FALTA DE MEMORIA]");
    return ptr;
}
void free(void* ptr){
    return kernel_free(ptr);
}
