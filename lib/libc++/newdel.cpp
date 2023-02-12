#include <stdvar.h>


void *operator new(size_t size)
{
    void* addr = malloc(size);
    return addr;
}
 
void *operator new[](size_t size)
{
    void* addr = malloc(size);
    return addr;
}
 
void operator delete(void* ptr)
{
    free(ptr);
}
 
void operator delete[](void* ptr)
{
    free(ptr);
}

/* sized deallocation functions */
void operator delete(void* ptr, size_t)
{
    free(ptr);
}
 
void operator delete[](void* ptr, size_t)
{
    free(ptr);
}
