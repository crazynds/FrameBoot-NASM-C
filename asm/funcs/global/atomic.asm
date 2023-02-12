[bits 64]

[global acquireLock]
[global releaseLock] 


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