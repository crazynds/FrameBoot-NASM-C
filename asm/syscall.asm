section .text
[bits 64]

[global syscall_handler]

syscall_handler:
    jmp $
    ;call syscall_executora
    sysret


