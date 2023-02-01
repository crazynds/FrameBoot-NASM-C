%define PAGE_PRESENT    (1 << 0)
%define PAGE_WRITE      (1 << 1)
%define WRITE_THROUGH   (1 << 3)
%define USER            (1 << 2)
%define PAGE            (PAGE_WRITE|PAGE_PRESENT)

[bits 16]
CheckCPU:
    ;checa se o cpuid é suportado
    pushfd     
    pop eax
    xor eax, 0x200000
    push eax
    popfd
    
    pushfd
    pop eax
    xor eax, ecx
    shr eax, 21
    and eax, 1                        ; Check whether bit 21 is set or not. If EAX now contains 0, CPUID isn't supported.
    test eax, eax
    jz $

    ;checa se a função 0x80000001 é suportada
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001               ; Check whether extended function 0x80000001 is available are not.
    jb $                    ; If not, long mode not supported.

    ;Testa se o bit de long mode pode ser setado
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29                 ; Test if the LM-bit, is set or not.
    jz $                    ; If not Long mode not supported.

    ; Switch to Long Mode.

; es:edi    Should point to a valid page-aligned 16KiB buffer, for the PML4, PDPT, PD and a PT.
; ss:esp    Should point to memory that can be used as a small (1 uint64) stack
SwitchToLongMode:
    ; Zero out the 16KiB buffer.
    ; Since we are doing a rep stosd, count should be bytes/4.

    push di                           ; REP STOSD alters DI.
    mov ecx, 0x1000
    xor eax, eax
    cld
    rep stosd
    pop di                            ; Get DI back.


    ; Build the Page Map Level 4.
    ; es:di points to the Page Map Level 4 table.
    lea eax, [es:di + 0x1000]         ; Put the address of the Page Directory Pointer Table in to EAX.
    or eax, PAGE ; Or EAX with the flags - present flag, writable flag.
    mov [es:di], eax                  ; Store the value of EAX as the first PML4E.


    ; Build the Page Directory Pointer Table.
    lea eax, [es:di + 0x2000]         ; Put the address of the Page Directory in to EAX.
    or eax, PAGE ; Or EAX with the flags - present flag, writable flag.
    mov [es:di + 0x1000], eax         ; Store the value of EAX as the first PDPTE.


    ; Build the Page Directory.
    lea eax, [es:di + 0x3000]         ; Put the address of the Page Table in to EAX.
    or eax, PAGE ; Or EAX with the flags - present flag, writeable flag.
    mov [es:di + 0x2000], eax         ; Store to value of EAX as the first PDE.

    push di                           ; Save DI for the time being.
    lea di, [di + 0x3000]             ; Point DI to the page table.
    xor eax,eax
    or eax,PAGE

    ; Build the Page Table.
.LoopPageTable:
    mov [es:di], eax
    add eax, 0x1000
    add di, 8
    cmp eax, 0x200000                 ; If we diad all 2MiB, end.
    jb .LoopPageTable

    pop di                            ; Restore DI.

    ; Disable IRQs
    mov al, 0xFF                      ; Out 0xFF to 0xA1 and 0x21 to disable all IRQs.
    out 0xA1, al
    out 0x21, al

    lidt [GDT.Null]                        ; Load a zero length IDT so that any NMI causes a triple fault. Carrega a idt em um lugar onde tem tamanho zerado

    ; Enter long mode.
    mov eax, 10100000b                ; Set the PAE and PGE bit.
    mov cr4, eax

    mov edx, edi                      ; Point CR3 at the PML4.
    mov cr3, edx

    mov ecx, 0xC0000080               ; Read from the EFER MSR.
    rdmsr

    or eax, 0x00000100                ; Set the LME bit.
    wrmsr

    mov ebx, cr0                      ; Activate long mode -
    or ebx,0x80000001                 ; - by enabling paging and protection simultaneously.
    mov cr0, ebx

    lgdt [GDT.Pointer]                ; Load GDT.Pointer defined below.


    jmp GDT.Code:INI_KERNEL             ; Load CS with 64 bit segment and flush the instruction cache


;Print:
;    pushad
;.PrintLoop:
;    lodsb                             ; Load the value at [@es:@si] in @al.
;    test al, al                       ; If AL is the terminator character, stop printing.
;    je .PrintDone
;    mov ah, 0x0E
;    int 0x10
;    jmp .PrintLoop                    ; Loop till the null character not found.

;.PrintDone:
;    popad                             ; Pop all general purpose registers to save them.
;    ret

;NoLongMode db "CPU NOT SUPORT 64 bits", 0x0A, 0x0D, 0