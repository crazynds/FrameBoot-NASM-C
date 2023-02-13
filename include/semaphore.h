#pragma once

#include <stdvar.h>

#define SEM_FAILED 1


struct sem_t{
    uint16 *addr_sem;
};

typedef struct sem_t sem_t;

// initilize semaphore
int    sem_init(sem_t *, int, unsigned int);
// destroy semaphore
int    sem_destroy(sem_t *);

int    sem_post(sem_t *);
// wait until has free space in semaphore
int    sem_wait(sem_t *);
// try to wait
int    sem_trywait(sem_t *);


//int    sem_getvalue(sem_t *, int *);


// SO - sempahore controlled

//sem_t *sem_open(const char *, int, ...);
//int    sem_close(sem_t *);
//int    sem_unlink(const char *);

