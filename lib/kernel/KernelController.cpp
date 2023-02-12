#include "KernelController.hh"

static KernelController kernelController;

KernelController *getKernelController(){
    return &kernelController;
}
