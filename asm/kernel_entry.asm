[bits 64]

section .text

[extern kmain]
[extern kloop]


start_64:
    sti
    call kmain
    ;jmp .helt
.loop:
    ;mov byte [TIME_SLICE],1
    hlt
    jmp .loop
.helt:
    hlt
    jmp .helt



