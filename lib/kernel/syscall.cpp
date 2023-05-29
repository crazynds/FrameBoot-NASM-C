#include <stdvar.h>
#include <kernel/gfx.h>



extern "C" void syscall_handler_global(){
    kprintStr(0,0,"Syscall executado!",TEXT_GREEN|BACKGROUND_BLUE);
}

