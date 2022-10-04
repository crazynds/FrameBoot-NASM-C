[bits 64]
section .text


global _start
_start:
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ds, ax

    mov rbp,0x0900
    mov rsp,rbp

    ; Blank out the screen to a blue color.
    mov edi, 0xB8000
    mov rcx, 500                      ; Since we are clearing uint64_t over here, we put the count as Count/4.
    mov rax, 0x1F201F201F201F20       ; Set the value to set the screen to: Blue background, white foreground, blank spaces.
    rep stosq                         ; Clear the entire screen.

    ;enable SSe
    call enable_sse


    jmp start_64


enable_sse:

    mov eax, 0x1
    cpuid
    test edx, 1<<25
    jz .noSSe
    test edx, 1<<26
    jz .noSSe
    mov rax, cr0
    and ax, 0xFFFB		;clear coprocessor emulation CR0.EM
    or ax, 0x2			;set coprocessor monitoring  CR0.MP
    mov cr0, rax
    mov rax, cr4
    or ax, 3 << 9		;set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    mov cr4, rax
    jmp .end

    .noSSe:
    jmp $   ;Trava a aplicação se não tiver suporte a SSE

    .end:

    ret
