#include <semaphore.h>
#include <stdvar.h>
#include <stdlib.h>


extern "C" int tryAcquireLock(uint16*);
extern "C" int acquireLock(uint16*);
extern "C" void releaseLock(uint16*);


int sem_init(sem_t *sem){
    sem->addr_sem = malloc(sizeof(uint16));
    *sem->addr_sem = 0;
}
int sem_destroy(sem_t *sem){
    free(sem->addr_sem);
}

int sem_post(sem_t *sem){
    releaseLock(sem->addr_sem);
    return 0;
}
int sem_wait(sem_t *sem){
    return acquireLock(sem->addr_sem);
}
int sem_trywait(sem_t *sem){
    return tryAcquireLock(sem->addr_sem);
}
