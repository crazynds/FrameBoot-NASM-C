[bits 64]

[global read_gsbase]
[global read_fsbase]
[global write_fsbase]
[global write_gsbase]

write_gsbase:
    mov eax,edi
    shr rdi,32
    mov edx,edi
    mov ecx,0xc0000101  ;gs base
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
