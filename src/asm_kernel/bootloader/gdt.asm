; GDT
[bits 16]

ALIGN 4
GDT:
.Null:
    ; Null Descriptor - should be present.
    dq 0
.Code:
    ; 64-bit code descriptor (exec/read).
    dq (1<<43)|(1<<44)|(1<<47)|(1<<53)|(1<<41)
.Data:
    ; 64-bit code descriptor (exec/read).
    dq (1<<44)|(1<<47)|(1<<41)
.Pointer:
    dw $ - GDT - 1                    ; 16-bit Size (Limit) of GDT.
    dd GDT                            ; 32-bit Base Address of GDT. (CPU will zero extend to 64-bit)
