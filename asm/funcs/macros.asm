
%macro includeBin 2
    SECTION .rodata
    GLOBAL %1
%1:
    %1_size: dq %1_size - %1
    incbin %2
    db 0


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
