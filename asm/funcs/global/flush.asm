[bits 64]
[global gdt_flush]
[global flush_tlb_address]


flush_tlb_address:
    invlpg [rdi]
    ret
gdt_flush:
    lgdt [rdi]
    ret
