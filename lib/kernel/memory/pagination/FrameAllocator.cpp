#include "FrameAllocator.hh"
#include <kernel/panic.h>

extern "C" int acquireLock(uint16*);
extern "C" void releaseLock(uint16*);

struct list_frame_t{
    memory_space data;
    volatile list_frame_t *next;
};

typedef struct list_frame_t list_frame_t;

list_frame_t* FrameAllocator::list_push(volatile list_frame_t *list,list_frame_t *newItem){
    if(list==nullptr || list->data.base > newItem->data.base){
        list_frame_t *tmp = newItem;
        tmp->next = list;
        list = tmp;
    }else{
        list->next = list_push(list->next,newItem);

        while(list->next !=nullptr && list->data.base+list->data.size >= list->next->data.base){
            list->data.size = ( list->next->data.base + list->next->data.size ) - list->data.base;
            volatile list_frame_t *tmp = list->next;
            list->next = tmp->next;
            releaseLock(&this->lock[1]);
            delete tmp;
            acquireLock(&this->lock[1]);
        }
    }
    return (list_frame_t*)list;
}


bool FrameAllocator::loadBuffer(){
    acquireLock(&this->lock[1]);
    while(qtd_buffer < MAX_BUFFER_FRAMES && this->avaliableMemory != nullptr){
        memory_space space = this->avaliableMemory->data;

        if(space.size < PAGE_SIZE){
            list_frame_t *next = (list_frame_t *)this->avaliableMemory->next;
            delete this->avaliableMemory;
            this->avaliableMemory = next;
        }else{
            this->addFrameInBuffer(space.base);
            this->avaliableMemory->data.base += PAGE_SIZE;
            this->avaliableMemory->data.size -= PAGE_SIZE;
        }
    }
    releaseLock(&this->lock[1]);
    return qtd_buffer>0;
}

void FrameAllocator::addFrameInBuffer(uint64 frame){
    acquireLock(&this->lock[0]);
    this->frame_buffer[this->qtd_buffer++] = frame & PAGE_MASK;
    releaseLock(&this->lock[0]);
}

void FrameAllocator::addMemorySpace(memory_space map){
    
    map.size = (map.size - (map.base & (~PAGE_MASK))) & PAGE_MASK;
    map.base = map.base & PAGE_MASK;
    while(map.size > PAGE_SIZE && this->qtd_buffer < MAX_BUFFER_FRAMES){
        this->addFrameInBuffer(map.base);
        map.base += PAGE_SIZE;
        map.size -= PAGE_SIZE;
    }

    if(map.size < PAGE_SIZE)return;
    list_frame_t *newObj = new list_frame_t;
    if(newObj==NULL){
        panic("[FrameAllocator::addMemorySpace]: Objeto alocado pelo new retornou NULL, falta de memória?");
    }
    newObj->data = map;
    acquireLock(&this->lock[1]);
    this->avaliableMemory = list_push(this->avaliableMemory,newObj);
    releaseLock(&this->lock[1]);
}

uint64 FrameAllocator::allocate(){
    if(this->qtd_buffer<=MAX_BUFFER_FRAMES/2 && !this->loadBuffer())
        return NULL;
    acquireLock(&this->lock[0]);
    uint64 frame = this->frame_buffer[--this->qtd_buffer];
    releaseLock(&this->lock[0]);
    return frame;
}


