#ifndef ASM_FUNCTIONS_H
    #define ASM_FUNCTIONS_H

    #include <stdvar.h>

    // If stack failed to aling this function will be called
    extern "C" void __stack_chk_fail(void);

    extern "C" void wait_port(void);
    extern "C" void timer_handler(void);
    extern "C" uint64 get_idt(void);
    extern "C" void sleep(uint64);
    
    extern "C" void outb(uint16 port, uint8 value);
    extern "C" uint8 inb(uint16 port);

#endif
