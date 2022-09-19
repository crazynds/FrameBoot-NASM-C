[org 0x7c00]
[bits 16]
    ;mov bx, MSG_REAL_MODE
    ;call print_string
    mov [0x7000], dl ; BIOS guarda o driver de boot em dl
    mov ebp, INI_PILHA    ; Iniciando a pilha na memoria
    mov esp, ebp

    clc
    int 0x12
    jc $

    mov bx, INI_KERNEL   ; onde o kernel vai ser colocado

    mov ah, 0x02	; BIOS read sector function
    mov al, SEG_KERNEL+1   ; number of sectors
    mov ch, 0x00	; cylinder no.
    mov dh, 0x00	; head no.
    mov cl, 0x02	; start from second sector
    int 0x13 		; BIOS interrupt
    ;pop dx
    ;cmp al,dh
    ;ja $ ; não carregou todo o kernel para a memoria
    cmp ax,0
    je $ ; Se ele retornou 0, significa que ele não conseguiu ler por algum erro.

    %include "boot/memory_maper.asm"

    call ChangeA20  ;Ativa memoria alta
    call CheckA20   ;Checa se memoria alta ta ativa

    mov edi, 0x2000 ;Aonde vai começar o paginamento 16 kb aproximadamente para 2 MB
    ;mov esp, INI_PILHA  ;Aonde vai começar a stack

    
    %include "boot/kernel_64.asm"

    ;jmp INI_KERNEL
    ;;


%include "boot/vars.asm"

%include "boot/a20_high_memory.asm"
%include "boot/gdt.asm"

; ADICIONAR A ASSINATURA DE BOOT
times 510-($-$$) db 0
dw 0xaa55
