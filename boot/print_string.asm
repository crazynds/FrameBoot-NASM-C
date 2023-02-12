print_string:
    pusha
    mov si, bx

    print:
        mov ah, 0x0e ; set scrolling teletype BIOS routine
        mov al, [si] ; set al reg. to character to print
        inc si
        or al, al
        jz end
        int 0x10 ; give interrupt to print on screen
        jmp print
        
    end:
        mov al, 13 	 	
        int 0x10
        mov al, 10
        int 0x10
        popa
        ret
    print2:
        push ax
        mov ah, 0x0e ; set scrolling teletype BIOS routine
        mov al, [bx] ; set al reg. to character to print
        int 0x10 ; give interrupt to print on screen
        pop ax
        ret
    
        
print_num:
    push bx
    push cx
    mov cx,0
erro:   
    bt ax,cx
    jc err1
    mov bx,er0
    jmp err0
err1:
    mov bx,er1
err0:
    call print2
    inc cx
    cmp cx,8
    jb erro
    call end
    pop cx
    pop bx
    ret


er1 db "1",0
er0 db "0",0
    
