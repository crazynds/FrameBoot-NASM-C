[bits 64]

[global acquireLock]
[global tryAcquireLock] 
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

tryAcquireLock:
    pause
    test word [rdi],1      ; Teste memory address before test with lock, if is already one, so isn't necessary to test with lock prefix
    jnz .cant_get

    lock bts word [rdi],0        ;Attempt to acquire the lock (in case lock is uncontended)
    xor eax,eax
    jnc .end
.cant_get:
    not eax
.end:
    ret

releaseLock:
    mov word [rdi],0
    ret