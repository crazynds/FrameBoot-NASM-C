[64 bits]
section .text

[global sleep]
[global outb]
[global inb]
[global gdt_fluh]
[global yield]
[global tss_flush]
[global __stack_chk_fail]

tss_flush:
    mov ax, 0x2B
;    ltr ax
    ret

gdt_fluh:
    lgdt [rdi]
    ret


    jmp enter_user_mode

    .continuos:

    jmp $

    ret

enter_user_mode:
    mov rdi,rsp
    push 0x10
    push rdi
    pushfq
    push 0x8
    push gdt_fluh.continuos

    iretq


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

yield:
    int 0x70
    ret

__stack_chk_fail:
    hlt
    jmp __stack_chk_fail

