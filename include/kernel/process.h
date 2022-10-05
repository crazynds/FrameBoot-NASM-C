#ifndef PROCESS_H
#define PROCESS_H

    #include <stdvar.h>

    struct process_state{
        uint64 heap_start;
        uint64 heap_end;
        uint64 stack_start;
        uint64 stack_end;
    };





#endif