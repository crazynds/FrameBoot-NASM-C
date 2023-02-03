[bits 64]

section .text

[extern kmain]
[extern kloop]
[global start_64]

start_64:
    cli
    call kmain
    jmp $
    ;jmp .helt
.loop:
    ;mov byte [TIME_SLICE],1
    hlt
    jmp .loop
.helt:
    hlt
    jmp .helt




