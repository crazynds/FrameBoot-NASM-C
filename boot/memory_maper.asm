[bits 16]
    mov ax,0x7000
    mov es,ax

    ;Faz a leitura do tamanho da memoria, vai ser destacado mais tarde
    mov di,(0xE000)  ;define o endereço base
    xor ebx,ebx ;zera ebx
    memory_map_repeat:
    mov edx,0x534D4150
    mov ecx,24
    mov eax,0xE820
    add di,cx
    int 0x15
    cmp ebx,0
    jnz memory_map_repeat