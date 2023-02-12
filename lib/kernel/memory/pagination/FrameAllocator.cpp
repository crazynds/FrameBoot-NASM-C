#include "FrameAllocator.hh"

extern "C" bool acquireLock(uint16*);
extern "C" void releaseLock(uint16*);

struct list_frame_t{
    memory_space data;
    list_frame_t *next;
};

typedef struct list_frame_t list_frame_t;

list_frame_t* list_push(list_frame_t *list,memory_space data){
    if(list==nullptr){
        list = new list_frame_t;
        list->next = nullptr;
        list->data = data;
    }else if(list->data.base > data.base){
        list_frame_t *tmp = list;
        list = new list_frame_t;
        list->next = tmp;
        list->data = data;
    }
    return list;
}

static FrameAllocator instance;


FrameAllocator& FrameAllocator::getInstance(){
    return instance;
}


bool FrameAllocator::loadBuffer(){
    while(qtd_buffer < MAX_BUFFER_FRAMES && this->avaliableMemory != nullptr){
        memory_space space = this->avaliableMemory->data;

        if(space.size < PAGE_SIZE){
            list_frame_t *next = this->avaliableMemory->next;
            delete this->avaliableMemory;
            this->avaliableMemory = next;
        }else{
            this->addFrameInBuffer(space.base);
            space.base += PAGE_SIZE;
            space.size -= PAGE_SIZE;
        }
    }
    return qtd_buffer>0;
}

void FrameAllocator::addFrameInBuffer(uint64 frame){
    this->frame_buffer[this->qtd_buffer++] = frame & PAGE_MASK;
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
    this->avaliableMemory = list_push(this->avaliableMemory,map);
}

uint64 FrameAllocator::allocate(){
    acquireLock(&this->lock);
    if(this->qtd_buffer<=0 && !this->loadBuffer())
        return NULL;
    uint64 frame = this->frame_buffer[--this->qtd_buffer];

    releaseLock(&this->lock);
    return frame;
}


