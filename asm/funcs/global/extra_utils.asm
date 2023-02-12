[bits 64]
section .text

[global outb]
[global inb]
[global __stack_chk_fail]

__stack_chk_fail:
    hlt
    jmp __stack_chk_fail

outb:
    mov edx,edi
    mov eax,esi
    out dx,al
    ret

inb:
    mov edx,edi
    xor eax,eax
    in al,dx
    ret
