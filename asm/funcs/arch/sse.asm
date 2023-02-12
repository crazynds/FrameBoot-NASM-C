[global enable_sse]

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
    ret

    .noSSe:
    hlt
    jmp .noSSe   ;Trava a aplicação se não tiver suporte a SSE
