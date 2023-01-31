/**
 * 
 * Esse arquivo ele vai ter uma pilha de memoria de 0xN -> 0xM e esse arquivo vai ser responsavel
 * por fazer chamadas para alocar mais frames ou liberar frames
 * 
 */
#include <stdlib.h>


void *operator new(unsigned long int size)
{
    return malloc(size);
}

void *operator new[](unsigned long int size)
{
    return malloc(size);
}

void operator delete(void *p)
{
    free(p);
}
void operator delete(void* p, long unsigned int)
{
    free(p);
}

void operator delete[](void *p)
{
    free(p);
}
void operator delete [](void* p, long unsigned int)
{
    free(p);
}