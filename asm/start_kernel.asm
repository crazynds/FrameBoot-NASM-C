section .entry
[bits 64]

[extern start_64]
[extern stack.start]
[extern kmain]
[extern enable_sse]
[extern write_fsbase]
[extern write_gsbase]


_start:
    ; Correct the kernel address to high memory
    mov rax, _start.upper_memory
    jmp rax
    .upper_memory:

    ; Initialize segment registers
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov ds, ax

    ; Clear fs and gs register segments
    xor rdi,rdi
    mov gs, edi
    mov fs, edi
    mov rbx,0xFFFFFF8000000000
    ; Set FS base and GS base to upper memory pointer
    mov rdi, rbx
    call write_gsbase
    mov rdi, rbx
    call write_fsbase

    mov rbp,stack.start
    mov rsp,rbp
    
    ; Blank out the screen to a blue color.
    mov edi, 0xB8000
    mov rcx, 500                      ; Since we are clearing uint64 over here, we put the count as Count/4.
    mov rax, 0x1F201F201F201F20       ; Set the value to set the screen to: Blue background, white foreground, blank spaces.
    rep stosq                         ; Clear the entire screen.

    ;enable SSE operations
    call enable_sse

    start_64:
    cli
    call kmain
    jmp $
    ;jmp .helt
.helt:
    hlt
    jmp .helt

