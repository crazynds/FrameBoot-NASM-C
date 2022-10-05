[extern idtSetInterruptGate]
[extern kprintStr]

%macro begin_isr 1
    mov rdi, %1
    mov rsi, isr_%1
    call idtSetInterruptGate
    jmp isr_%1.end
    isr_%1:
%endmacro

%macro end_isr 0
    iretq
    .end:
%endmacro

%macro generic_exception 2
    begin_isr %1
        cli
        mov rdi, 0
        mov rsi, 24
        mov rdx, .msg
        mov rcx, 0x1f
        call kprintStr
        jmp $
        .msg db %2, 0
    end_isr
%endmacro

%macro pushaq 0
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    pushfq
%endmacro

%macro popaq 0
    popfq
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax
%endmacro

%macro pushall 0
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
   pushfq
%endmacro

%macro popall 0
    popfq
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro
