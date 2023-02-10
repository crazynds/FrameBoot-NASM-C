[bits 64]
section .text

[global outb]
[global inb]
[global gdt_flush]
[global __stack_chk_fail]
[global flush_tlb_address]
[global acquireLock]
[global releaseLock] 


__stack_chk_fail:
    hlt
    jmp __stack_chk_fail

gdt_flush:
    lgdt [rdi]
    ret

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

flush_tlb_address:
    invlpg [rdi]
    ret

acquireLock:
    lock bts word [rdi],0        ;Attempt to acquire the lock (in case lock is uncontended)
    jc .spin_with_pause
    xor rax,rax
    ret
.spin_with_pause:
    pause                    ; Tell CPU we're spinning
    test word [rdi],1      ; Is the lock free?
    jnz .spin_with_pause     ; no, wait
    jmp acquireLock          ; retry


releaseLock:
    mov word [rdi],0
    ret