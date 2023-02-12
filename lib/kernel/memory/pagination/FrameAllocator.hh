#pragma once

#include "memory.h"


struct list_frame_t;

typedef struct list_frame_t list_frame_t;

list_frame_t* list_push(list_frame_t *list,memory_space data);


class FrameAllocator{
private:
    uint16 lock = 0;

    uint8 qtd_buffer = 0;
    list_frame_t *avaliableMemory = nullptr;
    uint64 frame_buffer[MAX_BUFFER_FRAMES];

    
    void addFrameInBuffer(uint64 frame);
    bool loadBuffer();

public:

    uint64 allocate();
    void addMemorySpace(memory_space map);

    static FrameAllocator& getInstance();

};


