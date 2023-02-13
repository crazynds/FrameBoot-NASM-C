#pragma once

#include "memory.h"


struct list_frame_t;

typedef struct list_frame_t list_frame_t;

list_frame_t* list_push(list_frame_t *list,memory_space data);


class FrameAllocator{
private:
    uint16 lock[2] = {0};

    volatile uint8 qtd_buffer = 0;
    list_frame_t *avaliableMemory = nullptr;
    uint64 frame_buffer[MAX_BUFFER_FRAMES+16];  // Pq mais 16? Pq existe uma chance minuscula de multi cores adicionarem ao mesmo tempo no buffer, então pra n dar problema ele tem uma parte para estourar

    
    list_frame_t* list_push(volatile list_frame_t *list,list_frame_t *newItem);
    void addFrameInBuffer(uint64 frame);
    bool loadBuffer();

public:

    uint64 allocate();
    void addMemorySpace(memory_space map);

};


