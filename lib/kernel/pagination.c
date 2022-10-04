#include <kernel/pagination.h>
#include <kernel/gfx.h>

#define KERNEL_PAGE_L4 ((uint64*)0x2000)
#define KERNEL_PAGE_L3(P4) ((uint64*)(KERNEL_PAGE_L4[P4]&0xFFFFFFFFFFFFFE00))
#define KERNEL_PAGE_L2(P4,P3) ((uint64*)(KERNEL_PAGE_L3(P4)[P3]&0xFFFFFFFFFFFFFE00))
#define KERNEL_PAGE_L1(P4,P3,P2) ((uint64*)(KERNEL_PAGE_L2(P4,P3)[P2]&0xFFFFFFFFFFFFFE00))
#define KERNEL_PAGE(P4,P3,P2,P1) ((uint64*)(KERNEL_PAGE_L1(P4,P3,P2)[P1]&0xFFFFFFFFFFFFFE00))

uint64 NEXT_PAGING_KERNEL=REAL_PAG_MEM;

uint64* getAddrMem(uint64 v){
    return (uint64*)(v&(~0x7ff));
}

uint64 getNextPagingKernel(){
    uint64 aux = (uint64)NEXT_PAGING_KERNEL;
    NEXT_PAGING_KERNEL+=4096;
    return aux;
}

uint8 getFlags(uint64 v){
    uint64 p=v&0xff;
    return (uint8)p;
}

void removePaging(uint16 l4,uint16 l3,uint16 l2,uint16 l1){
    if(l3>=L3 || l2>=L3 || l1>=L3)return;
    uint64 *pageL3=getAddrMem(KERNEL_PAGE_L4[l4]);
    uint64 *pageL2=getAddrMem(pageL3[l3]);
    uint64 *pageL1=getAddrMem(pageL2[l2]);

    pageL1[l1]=0x00;
}
void setPagingL3(uint16 l4,uint64 addr,uint16 flags){
    flags&=0x1FF;
    addr&=0xFFFFFFFFFFFFFE00;
    KERNEL_PAGE_L4[l4]=addr+flags;
}
void setPagingL2(uint16 l4,uint16 l3,uint64 addr,uint16 flags){
    uint64 *pageL3=getAddrMem(KERNEL_PAGE_L4[l4]);
    flags&=0x1FF;
    addr&=0xFFFFFFFFFFFFFE00;
    pageL3[l3]=addr+flags;
}
void setPagingL1(uint16 l4,uint16 l3,uint16 l2,uint64 addr,uint16 flags){
    uint64 *pageL3=getAddrMem(KERNEL_PAGE_L4[l4]);
    uint64 *pageL2=getAddrMem(pageL3[l3]);
    flags&=0x1FF;
    addr&=0xFFFFFFFFFFFFFE00;
    pageL2[l2]=addr+flags;
}
void setPaging(uint16 l4,uint16 l3,uint16 l2,uint16 l1,uint64 addr,uint16 flags,uint16 len){
    uint64 *pageL3=getAddrMem(KERNEL_PAGE_L4[l4]);
    uint64 *pageL2=getAddrMem(pageL3[l3]);
    uint64 *pageL1=getAddrMem(pageL2[l2]);
    flags&=0x1FF;
    addr&=0xFFFFFFFFFFFFFE00;
    len--;
    pageL1[l1]=(addr)|flags;
    for(;len>0;len--){
        pageL1[l1+len]=(addr+0x1000*len)|flags;
    }
}
void page_fault_handler(uint32 error,uint64 endereco){
    if(error&8){
        kprintStr(0,24,"Erro RSVD" ,0x4f);
    }else if(error&16){
        kprintStr(0,24,"Erro I/D" ,0x4f);
    }else kprintStr(0,24,FAULT[error&7],0x4f);
    kprintnum(0,23,endereco);
}

void setFault(){
    FAULT[0]="Supervisory process tried to read a non-present page entry";
    FAULT[1]="Supervisory process tried to read a page and caused a protection fault";
    FAULT[2]="Supervisory process tried to write to a non-present page entry";
    FAULT[3]="Supervisory process tried to write a page and caused a protection fault";
    FAULT[4]="User process tried to read a non-present page entry";
    FAULT[5]="User process tried to read a page and caused a protection fault";
    FAULT[6]="User process tried to write to a non-present page entry";
    FAULT[7]="User process tried to write a page and caused a protection fault";
}

uint64 loadMemoryInformation(){
    struct{
        uint64 base;
        uint64 size;
        uint32 type;
        uint32 extendedAtb;
    }__attribute__((packed)) *mem = (void*)(0x7E000+24);
    uint64 maxMemory = 0x100000;

    for(int x=0;mem->size!=0;x++){
        
        kprintStr(1,10+x,"Memoria",0x0f);

        kprinthex(15,10+x,mem->base);
        kprinthex(30,10+x,mem->size);
        kprintnum(45,10+x,mem->type);
        kprintnum(60,10+x,mem->extendedAtb);
        if(mem->type==1 && mem->base>=100000){
            if(mem->base+mem->size>maxMemory)maxMemory= mem->base+mem->size;
            //addMemoryEntry(mem->base,mem->size);
        }
        mem ++;
    }
    return maxMemory&0xFFFFFFFFFFFFFE00;
}

void setupPagination(){
    setFault();
    uint64 maxMemory = loadMemoryInformation();
    
    uint64 base = getNextPagingKernel();
    //cria um conjunto de paginas para remapeado os primeiros 2 MB do kernel.
    setPagingL3(1,base,PRESENT|RW|WRITE_THROUGH);
    setPagingL2(1,0,getNextPagingKernel(),PRESENT|RW|WRITE_THROUGH);
    setPagingL1(1,0,0,getNextPagingKernel(),PRESENT|RW|WRITE_THROUGH);
    setPaging(1,0,0,0,0,PRESENT|RW|WRITE_THROUGH,512);
    //Move o endereço que já tem os mapas para a posição 0
    setPagingL3(0,base,PRESENT|RW|WRITE_THROUGH);
    //redefine a página L3 para vazio
    setPagingL3(1,0,0);

    uint64 remap = 0x200000;

    for(int x=0;x<512 && maxMemory>remap;x++){
        if(x!=0){
            setPagingL3(x,getNextPagingKernel(),PRESENT|RW|WRITE_THROUGH);
        }
        for(int y=0;y<512 && maxMemory>remap;y++){
            if(y!=0 || x!=0){
                setPagingL2(x,y,getNextPagingKernel(),PRESENT|RW|WRITE_THROUGH);
            }
            for(int z=0;z<512 && maxMemory>remap; z++){
                if(z!=0 || y!=0 || x!=0){
                    setPagingL1(x,y,z,getNextPagingKernel(),PRESENT|RW|WRITE_THROUGH);
                    uint16 val = ((maxMemory-remap)/0x1000<512)?((maxMemory-remap)/0x1000):512;
                    setPaging(x,y,z,0,remap,PRESENT|RW|WRITE_THROUGH,val);
                    remap+=0x1000*val;
                }
            }
        }
    }

}
