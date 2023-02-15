#include <kernel/panic.h>
#include <stdlib.h>

void* malloc(size_t){
    panic("MALLOC IS NOT DEFINED");
    return NULL;
}
void* calloc(size_t, size_t){
    panic("CALLOC IS NOT DEFINED");
    return NULL;
}
void* realloc(void*, size_t){
    panic("REALLOC IS NOT DEFINED");
    return NULL;
}
void free(void*){
    panic("FREE IS NOT DEFINED");
}
