#include <kernel/gfx.h>
#include <io.h>

struct Char{
    char c;
    int8 color;
};

static const size_t NUM_COLS = 80;
static const size_t NUM_ROWS = 25;
struct Char *VGA_MEM=(struct Char *)0xb8000;


void disableCursor(){
	outb(0x0A,0x3D4);
	outb(0x20,0x3D5);
}
void enableCursor ( uint8 cursor_start ,  uint8 cursor_end ) {
	outb ( 0x0A , 0x3D4 ) ;
	outb ( ( inb ( 0x3D5 )  &  0xC0 )  | cursor_start , 0x3D5 ) ;

	outb ( 0x0B , 0x3D4 ) ;
	outb ( ( inb ( 0x3D5 )  &  0xE0 )  | cursor_end , 0x3D5 ) ;
}
void setCursorPosition(uint16 position){
    outb(0x0F,0x3D4);
    outb((unsigned char)(position&0xff),0x3D5);
    outb(0x0E,0x3D4);
    outb((unsigned char)((position>>8)&0xff),0x3D5);
}
uint16 getCursorPosition(){
    uint16 pos = 0;
    outb(0x0F,0x3D4);
    pos |= inb(0x3D5);
    outb(0x0E,0x3D4);
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


int gettam_num(int64 num){
    int count = 0;
    if(num == 0)return 1;
    if(num<0)num*=-1;
    while(num > 0){
        count++;
        num = num/10;
    }
    return count;
}
int gettam_hex(int64 num){
    int count = 0;
    if(num == 0)return 1;
    if(num<0)num*=-1;
    while(num > 0){
        count++;
        num = num/16;
    }
    return count;
}
void int_asc(int64 number, char *numberArray){
    int n = gettam_num(number);
    int i=n-1;
    char neg =0;
    if(number<0){
        numberArray[0]='-';
        neg=1;
        number*=-1;
    }
    numberArray[n+neg]='\0';
    for (; i >= 0; --i, number /= 10 ){
        numberArray[i+neg] = '0' + number % 10;
    }
}
void hex_asc(int64 number, char *numberArray){
    int n = gettam_hex(number);
    int i=n-1;
    char neg =0;
    if(number<0){
        numberArray[0]='-';
        neg=1;
        number*=-1;
    }
    numberArray[n+neg+2]='\0';
    numberArray[0]='0';
    numberArray[1]='x';
    for (; i >= 0; --i, number /= 16 ){
        char a = number % 16;
        if(a>9){
            a = 'A'+(a-10);
        }else{
            a = '0'+a;
        }
        numberArray[i+neg+2] = a;
    }
}
void kprintnum(int a,int b,int64 x){
    char c[21];
    int_asc(x, c);
    kprintStr(a,b,c,0x0f);
}
void kprinthex(int a,int b,int64 x){
    char c[21];
    hex_asc(x, c);
    kprintStr(a,b,c,0x0f);
}
void kprintnum2(int64 x){
    char c[21];
    int_asc(x, c);
    kprintStr(70,0,c,0x0f);
}
uint8 val=0;
void kprintnum3(int64 x){
    char c[21];
    int_asc(x, c);
    kprintStr(6,7+val++,c,0x0f);
}
