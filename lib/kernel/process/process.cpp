
#include "Process.hh"
#include "../KernelController.hh"



void setupProcess(KernelController *kernel){
    Process *p = new Process(true);
    kernel->setCurrentProcess(p);
}