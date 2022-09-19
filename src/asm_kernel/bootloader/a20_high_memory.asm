[bits 16]

WaitKBC:
   mov cx,0ffffh
   A20L:
   in al,64h
   test al,2
   loopnz A20L
ret

ChangeA20:
   call WaitKBC
   mov al,0d1h
   out 64h,al
   call WaitKBC
   mov al,0dfh ; use 0dfh to enable and 0ddh to disable.
   out 60h,al
ret

CheckA20:
    clc
    PUSH ds

    XOR ax,ax 
    MOV es,ax 
    NOT ax 
    MOV ds,ax 
    MOV ah,[es:0] 
    CMP ah,[ds:10h] 
    JZ A20_OFF 
    INC ah 
    MOV [es:0],ah 
    CMP [ds:10h],ah 
    PUSHF 
    DEC ah 
    MOV [es:0],ah 
    POPF 
    JZ A20_OFF 
    
    POP ds
    RET

A20_OFF: 
    jnc $           ;Se não tiver, trava o computador
