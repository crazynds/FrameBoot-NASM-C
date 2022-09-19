# FrameBoot-NASM-C
Simplest framework for boot your application in x86_64 


# Framework Explanation

### App folder

Contains all your source code in C language.

### Asm folder

Contains all your source code in Asm language.

### Boot

Boot is the source code used to boot in the hardware, and setup your cpu to 64 bit mode. This code also loads your application to memory and after 
finish all setup call to start function in the kernel_entry file.

### Include

All header to import in application using #import <stdlib.h>

### Lib

All source code for the imports.


# Build

- Build & Test & Clear:
```bash
make
```
- Build & Test:
```bash
make test
```
- Make ISO:
```bash
make iso
```

