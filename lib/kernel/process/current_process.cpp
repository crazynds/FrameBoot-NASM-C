
#include <kernel/process.h>


static struct process_state currentState;

void setupProcess(){
    currentState.heap_start = 0x100000;
    currentState.heap_end = 0x200000;
    currentState.stack_start = 0x0900;
    currentState.stack_end = 0x01FFF;
}
