#pragma once

#include "memory.h"


struct list_frame_t{
    memory_space data;
    list_frame_t *next;
};

typedef struct list_frame_t list_frame_t;

list_frame_t* list_push(list_frame_t *list,memory_space data);


class FrameManager{
private:
    uint8 qtd_buffer = 0;
    list_frame_t *avaliableMemory = nullptr;
    uint64 frame_buffer[MAX_BUFFER_FRAMES];

    static FrameManager instance;

    bool loadBuffer();

public:

    void addFrameInBuffer(uint64 frame);
    void addMemoryMap(memory_space map);

    ptr_t allocate();
    void free(ptr_t val);

    static FrameManager& getInstance(){
        return instance;
    }

};


