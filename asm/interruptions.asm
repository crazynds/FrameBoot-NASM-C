section .text
[bits 64]
%include "asm/funcs/macros.asm"

[global idt_register_isrs]

[extern pageFaultHandler]
[extern kprintChar]

idt_register_isrs:
  generic_exception 0,  "divide by zero"
  generic_exception 1,  "debug"
  generic_exception 2,  "non-maskable interrupt"
  generic_exception 3,  "breakpoint"
  generic_exception 4,  "overflow"
  generic_exception 5,  "bound range exceeded"
  generic_exception 6,  "invalid opcode"
  generic_exception 7,  "device not available"
  generic_exception 8,  "double fault"
  generic_exception 10, "invalid tss"
  generic_exception 11, "segment not present"
  generic_exception 12, "stack segment fault"
  generic_exception 13, "general protection fault"
  generic_exception 14, "page fault"
  

  ; PAGE FAULT 14
  ;begin_isr 14
  ;  pushaq
  ;  mov rsi,cr2
  ;  call pageFaultHandler
  ;  popaq
  ;end_isr

  ; IRQ 0 - PIT
  begin_isr 32
    ; 6 valores armazenados na stack
    ; sendo o 3 valor o ponteiro para retornar para o processo normal
    ; testar usando apenas os 3 primeiros
    ; depois progredir
    push rax
    mov al, 0x20
    out 0xa0, al
    out 0x20, al
    pop rax
  end_isr

  ; IRQ 1 - keyboard
  begin_isr 33
    pushall

    in al,0x60
    mov rdi,10
    mov rsi,10
    mov dl,al;
    mov rcx,0x1f;
    call kprintChar;

    push rax
    mov al, 0x20
    out 0xa0, al
    out 0x20, al
    pop rax;

    popall
  end_isr

  ret

