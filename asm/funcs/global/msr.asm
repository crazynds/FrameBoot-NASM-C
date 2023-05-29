[bits 64]

[global read_gsbase]
[global read_fsbase]
[global read_kernelgsbase]
[global write_fsbase]
[global write_gsbase]
[global write_kernelgsbase]
[global setup_syscall]

[extern syscall_handler]


setup_syscall:
    ; Set (0xC0000081) bits 32-47 are kernel segment base, bits 48-63 are user segment base (ON GDT)
    mov ecx, 0xC0000081
    ; setup with kernel segment inside and outside syscall
    mov edx, 0x00080008
    wrmsr

    ; (0xC0000082) The kernel's RIP SYSCALL entry for 64 bit software
    mov ecx, 0xC0000082
    mov rdx, syscall_handler
    mov eax,edx
    shr rdx,32
    wrmsr

    
    ; Set IA32_EFER.SCE (0xC0000080) and SCE (SYSCALL Enable) is its 0th bit
    mov ecx,0xC0000080  ;fs base
    xor rax,rax
    rdmsr
    or rax,1
    wrmsr

    ret

write_gsbase:
    mov eax,edi
    shr rdi,32
    mov edx,edi
    mov ecx,0xc0000101  ;gs base
    wrmsr
    ret

write_kernelgsbase:
    mov eax,edi
    shr rdi,32
    mov edx,edi
    mov ecx,0xC0000102  ;kernel gs base
    wrmsr
    ret

write_fsbase:
    mov eax,edi
    shr rdi,32
    mov edx,edi
    mov ecx,0xc0000100  ;fs base
    wrmsr
    ret

read_fsbase:
    mov ecx,0xc0000100  ;fs base
    xor rax,rax
    rdmsr
    shl rdx,32
    or rax,rdx
    ret

read_gsbase:
    mov ecx,0xc0000101  ;gs base
    xor rax,rax
    rdmsr
    shl rdx,32
    or rax,rdx
    ret

read_kernelgsbase:
    mov ecx,0xC0000102  ;kernel gs base
    xor rax,rax
    rdmsr
    shl rdx,32
    or rax,rdx
    ret
