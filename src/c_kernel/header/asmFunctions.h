#ifndef ASM_FUNCTIONS_H
    #define ASM_FUNCTIONS_H

    #include "Variaveis.h"

    // If stack failed to aling this function will be called
    extern void __stack_chk_fail(void);

    extern void wait_port(void);
    extern void timer_handler(void);
    extern uint64 get_idt(void);
    extern void sleep(uint64);

    extern void outb(uint16 port, uint8 value);
    extern uint8 inb(uint16 port);

#endif
