
#include <kernel/gfx.h>
#include <io.h>
#include <System.h>

#define IDT_INTERRUPT_GATE 0xee
#define IDT_TRAP_GATE      0x8f
#define IDT_TASK_GATE      0x85


extern void idt_register_isrs();

typedef struct {
    uint16 offset_0_15;
    uint16 segment;
    uint8 ist;
    uint8 type;
    uint16 offset_16_31;
    uint32 offset_32_63;
    uint32 _reserved;
} __attribute__((packed))
idt_entry;

typedef struct {
    uint16 limit;
    idt_entry* base;
} __attribute__((packed))
idtr_t;

static idt_entry idt[256];

void idt_set_interrupt_gate(uint8 vector, uint64 handler)
{
    idt[vector].offset_0_15 = handler;
    idt[vector].segment = 0x8;
    idt[vector].ist = 0;
    idt[vector].type = IDT_INTERRUPT_GATE;
    idt[vector].offset_16_31 = handler >> 16;
    idt[vector].offset_32_63 = handler >> 32;
    idt[vector]._reserved = 0;
}
void idt_set_trap_gate(uint8 vector, uint64 handler)
{
    idt[vector].offset_0_15 = handler;
    idt[vector].segment = 0x8;
    idt[vector].ist = 0;
    idt[vector].type = IDT_TRAP_GATE;
    idt[vector].offset_16_31 = handler >> 16;
    idt[vector].offset_32_63 = handler >> 32;
    idt[vector]._reserved = 0;
}
void idt_set_task_gate(uint8 vector, uint64 handler)
{
    idt[vector].offset_0_15 = handler;
    idt[vector].segment = 0x8;
    idt[vector].ist = 0;
    idt[vector].type = IDT_TASK_GATE;
    idt[vector].offset_16_31 = handler >> 16;
    idt[vector].offset_32_63 = handler >> 32;
    idt[vector]._reserved = 0;
}
static inline void stopInt(){
    __asm__ ("cli");
}
static inline void startInt(){
    __asm__ ("sti");
}


void remap_irqs(){
    outb(0x11, 0x20);
    outb(0x11, 0xa0);
    outb(0x20, 0x21);
    outb(0x28, 0xa1);
    outb(0x04, 0x21);
    outb(0x02, 0xa1);
    outb(0x01, 0x21);
    outb(0x01, 0xa1);
    outb(0x00, 0x21);
    outb(0x00, 0xa1);
}


void setTimer(uint16 freq){
    uint32 div=1193180/freq;
    uint8 l=(uint8)(div&0xff);
    uint8 h=(uint8)((div>>8)&0xff);
    outb(0x36, 0x43);
    outb(l, 0x40);
    outb(h, 0x40);
}
void idtInit(){
    remap_irqs();
    memzero(idt, sizeof(idt_entry) * 256);
    idt_register_isrs();
    idtr_t idtr = { .base = idt, .limit = sizeof(idt_entry) * 256 - 1 };
    __asm__ volatile("lidt %0" :: "m"(idtr));

    setTimer(60);
    // 20 - 30 hz
    disableCursor();
    enableCursor (15,15);
    setCursorPosition(0);

}
/*void enableIRQ(char port)
{
    interrupt_gates=(port|(interrupt_gates^0xff))^0xff;
    write_port(PIC1_DATA , interrupt_gates);
}
void disableIRQ(char port)
{
    interrupt_gates=(port^(interrupt_gates^0xff))^0xff;
    write_port(PIC1_DATA , interrupt_gates);
}*/
