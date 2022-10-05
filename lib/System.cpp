#include <stdvar.h>


void memzero(void* ptr, size_t size){
    if (size % 8 == 0) {
        size /= 8;
        uint64* p64 =(uint64*) ptr;
        for (size_t i = 0; i < size; i++) {
            p64[i] = 0;
        }
    } else {
        uint8* p8 =(uint8*) ptr;
        for (size_t i = 0; i < size; i++) {
            p8[i] = 0;
        }
    }
}