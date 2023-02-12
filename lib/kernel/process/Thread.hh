#pragma once
#include <stdvar.h>


typedef struct{
    uint64 rax,rbx,rcx,rdx; // General propose registers
    uint64 r8,r9,r10,r11,r12,r13,r14,r15;
    uint64 rsp,rbp; // Stack pointers

    uint64 fs_base; // Private data thread
} cpu_state_t;

typedef struct{
    ptr_t stack_start;
    uint64 size;
} stack_t;


class Thread{
private:
    cpu_state_t state;
    stack_t stack;
public:


};