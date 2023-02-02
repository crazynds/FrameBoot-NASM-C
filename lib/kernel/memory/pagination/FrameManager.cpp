#include "FrameManager.hh"


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


bool FrameManager::loadBuffer(){
    while(qtd_buffer < MAX_BUFFER_FRAMES && this->avaliableMemory != nullptr){
        memory_space space = this->avaliableMemory->data;
        uint64 qtdFrames = space.size/PAGE_SIZE;

        if(qtdFrames == 0){
            list_frame_t *next = this->avaliableMemory->next;
            delete this->avaliableMemory;
            this->avaliableMemory = next;
        }else{
            this->addFrameInBuffer(space.base & PAGE_MASK);
            space.base += PAGE_SIZE;
            space.size -= PAGE_SIZE;
        }
    }
    return qtd_buffer>0;
}

void FrameManager::addFrameInBuffer(uint64 frame){
    this->frame_buffer[this->qtd_buffer++] = frame;
}

void FrameManager::addMemoryMap(memory_space map){
    if(map.size <= 0)return;
    map.base = map.base & (~0xFFF);
    this->avaliableMemory = list_push(this->avaliableMemory,map);
}

ptr_t FrameManager::allocate(){
    if(this->qtd_buffer<=0 && !this->loadBuffer())
        return NULL;
    return (ptr_t)this->frame_buffer[--this->qtd_buffer];
}

void FrameManager::free(ptr_t val){
    uint64 frame = (uint64)val & (PAGE_MASK);
    this->addMemoryMap({
        frame, PAGE_SIZE
    });
}


