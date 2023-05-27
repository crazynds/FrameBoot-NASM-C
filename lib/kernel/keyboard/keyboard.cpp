#include <stdvar.h>

const uint8 keymap_br[128]={0,
0,//Esc
'1','2','3','4','5','6','7','8','9','0','-','=','\b',//backspace 13
'\t',//tab
'q','w','e','r','t','y','u','i','o','p',
0,//acento 'á é ´ ' 25
'[','\n',
0,//LEFT CONTROL 28
'a','s','d','f','g','h','j','k','l',0,
0,//aceito tiu ~ 40
'\'',
0,//lEFT SHIFT 42
']','z','x','c','v','b','n','m',',','.',';',
0,//SHIFT direito 54
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
0, // direita setinha
'+',
0, // end key
0,0,0,0,0,0,0,0,0,0
};

char lastkey = ' ';

void keyboard_input(uint8 keycode){
    if(keymap_br[keycode]==0)return; // se o código n é um caractere, por enquanto não é tratado
    if(keycode&1<<7)return; // Se o código é um release, por enquanto não é tratado
    lastkey = keymap_br[keycode];
}