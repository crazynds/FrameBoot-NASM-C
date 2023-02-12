#ifndef KSETUP_H
    #define KSETUP_H

    #include <kernel/class.h>

    void setupPagination(KernelController *);
    void setupGDT(KernelController *);
    void setupIDT(KernelController *);

#endif
