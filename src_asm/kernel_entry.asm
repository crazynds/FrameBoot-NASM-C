
[bits 64]
%include "src/asm_kernel/funcs/start_kernel.asm"

%include "src/asm_kernel/funcs/macros.asm"
%include "src/asm_kernel/funcs/extra_utils.asm"

[extern kmain]
[extern kloop]

[global __stack_chk_fail]

start_64:
    sti
    call kmain
    ;jmp .helt
.loop:
    ;mov byte [TIME_SLICE],1
    hlt
    jmp .loop
.helt:
    hlt
    jmp .helt


__stack_chk_fail:
    hlt
    jmp __stack_chk_fail

