
#include <stdvar.h>

static uint16 lock = 0;

extern "C" bool acquireLock(uint16*);
extern "C" void releaseLock(uint16*);


int liballoc_lock(){
    return acquireLock(&lock);
}

int liballoc_unlock(){
    releaseLock(&lock);
    return 0;
}

void* liballoc_alloc(int){
    return NULL;
}

int liballoc_free(void*,int){
    return 0;
}