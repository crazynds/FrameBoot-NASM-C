: Script para rodar no windows

: requisitos: 
    : WSL instalado mom os programas necessários para compilar (make gcc g++ nasm), 
    : QEMU instalado no windows e no path, 
    : programas padrões GNU instalados no windows (make, rm, rmdir)

make clean
wsl make build
make test