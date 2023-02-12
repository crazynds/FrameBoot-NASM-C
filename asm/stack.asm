section .stack

[global stack.start]

stack:
    .end:
times 4096 db 0
    .start:
