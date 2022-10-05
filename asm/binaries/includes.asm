
%macro IncludeBin 2
    SECTION .rodata
    GLOBAL %1
%1:
    incbin %2
    db 0
    .%1_size: dq $ - %1
%endmacro

IncludeBin Test, "storage/test.txt"



