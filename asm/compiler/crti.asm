section .init
[global _init]
_init:
    push rax
    push rbp
    mov rbp,rsp

section .fini
[global _fini]
_fini:
    push rax
    push rbp
    mov rbp,rsp
