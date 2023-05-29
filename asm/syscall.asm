section .text
[bits 64]

[global syscall_handler]
[extern syscall_handler_global]

syscall_handler:
    ;call syscall_handler_global
    jmp $
    sysret


