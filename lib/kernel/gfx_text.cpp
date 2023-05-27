#include <kernel/gfx.h>
#include <kernel/asmFunctions.hh>
#include <kernel/address.h>
#include <stdio.h>

struct Char{
    char c;
    int8 color;
};

static const size_t NUM_COLS = 80;
static const size_t NUM_ROWS = 25;
struct Char *VGA_MEM=(struct Char *)VGA_ADDRESS;


void disableCursor(){
	outb(0x3D4,0x0A);
	outb(0x3D5,0x20);
}
void enableCursor ( uint8 cursor_start ,  uint8 cursor_end ) {
	outb ( 0x3D4, 0x0A ) ;
	outb ( 0x3D5, ( inb ( 0x3D5 )  &  0xC0 )  | cursor_start  ) ;

	outb ( 0x3D4, 0x0B ) ;
	outb ( 0x3D5, ( inb ( 0x3D5 )  &  0xE0 )  | cursor_end  ) ;
}
void setCursorPosition(uint16 position){
    outb(0x3D4,0x0F);
    outb(0x3D5, (unsigned char)(position&0xff));
    outb(0x3D4,0x0E);
    outb(0x3D5, (unsigned char)((position>>8)&0xff));
}
uint16 getCursorPosition(){
    uint16 pos = 0;
    outb(0x3D4,0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4,0x0E);
    pos |= ((uint16)inb(0x3D5)) << 8;
    return pos;
}

extern "C" void kprintChar(int x,int y,char c,int8 color){
    uint16 p = NUM_COLS*y+x;
    VGA_MEM[p]=(struct Char){
        c,
        color
    };
    setCursorPosition(p+1);
}
void clrscr(color col){
    for ( size_t r = 0 ; r < NUM_ROWS ; r++ ){
        for ( size_t c = 0 ; c < NUM_COLS ; c++ ){
            kprintChar(c, r, ' ',col);
        }
    }
}

extern "C" void kprintStr(int x,int y,const char *s,color col){
    x%=NUM_COLS;
    y%=NUM_ROWS;
    while(*s!='\0'){
        if(*s=='\n'){
            y+=1;
            x=0;
        }else{
            kprintChar(x,y,*s,col);
            x++;
        }
        if(x>=(int)NUM_COLS){
            y++;
            x%=NUM_COLS;
        }
        if(y>=(int)NUM_ROWS){
            y%=NUM_ROWS;
            x=0;
        }
        s++;
    }
}

void kprintnum(int a,int b,int64 x){
    char c[40];
    sprintf(c,"%ld",x);
    kprintStr(a,b,c,0x0f);
}
void kprinthex(int a,int b,uint64 x){
    char c[40] = {'0','x','0'};
    sprintf(c,"%x",x);
    kprintStr(a,b,c,0x0f);
}
void kprintnum2(int64 x){
    char c[40];
    sprintf(c,"%ld",x);
    kprintStr(70,0,c,0x0f);
}
uint8 val=0;
void kprintnum3(int64 x){
    char c[40];
    sprintf(c,"%ld",x);
    kprintStr(6,7+val++,c,0x0f);
}
