#include "../header/gfx.h"

short *VGA_MEM=(short *)0xb8000;

void clrscr(){
    for ( int r = 0 ; r < 30 ; r++ ){
        for ( int c = 0 ; c < 80 ; c++ ){
            kprintChar(c, r, ' ',0x0F);
        }
    }
}

inline void kprintChar(int x,int y,char c,int8 color){
    VGA_MEM[80*y+x]=c|(color<<8);
}

void kprintStr(int x,int y,char *s,int8 color){
    while(*s!='\0'){
        kprintChar(x,y,*s,color);
        x++;
        s++;
    }
}

void kprint(char *s){
    kprintStr(0,10,s,0x1f);
}


int gettam_num(int64 num){
    int count = 0;
    if(num == 0)return 1;
    while(num > 0){
        count++;
        num = num/10;
    }
    while(num<0){
        count++;
        num = num/10;
    }
    return count;
}
void int_asc(int64 number, char *numberArray){
    int n = gettam_num(number);
    int i;
    numberArray[n]='\0';
    for ( i = n-1; i >= 0; --i, number /= 10 ){
        numberArray[i] = '0' + number % 10;
    }
}
void kprintnum(int a,int b,int64 x){
    char c[21];
    int_asc(x, c);
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
