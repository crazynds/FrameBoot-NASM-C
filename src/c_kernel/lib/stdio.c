#include "../header/Variaveis.h"
// #include "../header/Thread.h"
#include "stdio.h"
#include "System.h"

//uint32 _sprintf(char *out, char *str,const void *var_list);

// void _printf(char *str,void *var_list){
//   char buffer[256];
//   _sprintf(&buffer,str,var_list);
//   output_stream *out=NOW_THREAD->output;
//   ((void (*)(void*,char*))out->funcOut)(out,buffer);
// }

// void putchar(char c){
//   char str[2]={0,'\0'};
//   str[0]=c;
//   output_stream *out=NOW_THREAD->output;
//   ((void (*)(void*,char*))out->funcOut)(out,str);
// }


#define ESC 0
#define BACKSPACE 13
#define LEFT_CONTROL 28
#define LEFT_SHIFT 42
#define RIGTH_SHIFT 54
#define CAPSLOCK 57

uint8 keymap_br[128]={0,0,//Esc
'1','2','3','4','5','6','7','8','9','0','-','=','\b',//backspace 13
'\t',//tab
'q','w','e','r','t','y','u','i','o','p',0,//acento 'á é ´ ' 25
'[','\n',0,//LEFT CONTROL 28
'a','s','d','f','g','h','j','k','l',0,0,//aceito tiu ~ 40
'\'',0,//lEFT SHIFT 42
']','z','x','c','v','b','n','m',',','.',';',0,//SHIFT direito 54
0,// ??
0,//ALT Gr
' ',//space
0,//capslock 58
0,0,0,0,0,0,0,0,0,0, // f1-f10 68
0, // Scrol lock
0,
0, // home keycode
0, //flechinha pra cima 72
0, //page up
'-',
0, // left setinha
0,
0, // direitia setinha
'+',
0, // end key
0,0,0,0,0,0,0,0,0,0
};
static uint8 buffer[8]={};
static uint64 keyboard_vez=0;

static uint16 getCaracTime(uint64 time){
  if(keyboard_vez>time){
    uint64 aux=keyboard_vez-time;
    if(aux>=8){
      return (8<<8)+buffer[7];
    }else return ((aux+1)<<8)+buffer[aux-1];
  }else return 0;
}

char getChar(){
  uint64 time=keyboard_vez;
  uint16 carac=0;
  while(carac==0)carac=getCaracTime(time);
  return (char)carac;
}

char getch(){
  return getChar();
}


// char* gets(char *buff){
//   uint64 time=keyboard_vez;
//   uint16 aux;
//   uint32 x=0;
//   do{
//     aux=getCaracTime(time);

//     if(aux!=0){
//       uint8 aux2=(uint8)aux>>8;
//       if(aux2>0)time=keyboard_vez-aux2-1;
//       else time=keyboard_vez;
//       buff[x++]=(char)aux;
//       printf("%c",aux);
//     }else if(aux=='\b'){
//       if(x>0){
//         printf("%c",aux);
//         x--;
//       }
//     }else{
//       aux=~aux;
//     }
//   }while(((char)aux)=='\0');
//   return buff;
// }


void keyboard(uint8 keycode){
    if(keymap_br[keycode]==0)return;
    if(keycode&1<<7)return;
    keyboard_vez++;
    int x;
    for(x=7;x>0;x--)buffer[x]=buffer[x-1];
    buffer[0]=keymap_br[keycode];
}
