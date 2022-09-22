#ifndef GFX_H
    #define GFX_H

        #include <stdvar.h>


        //Utilitários para kprintnum
        void int_asc(int64 num, char *numero);
        int gettam_num(int64 num);
        uint64 asc_int(char *numero);

        void clrscr();
        void kprintChar(int x,int y,char c,int8 color);
        void kprintStr(int x,int y,char *s,int8 color);
        void kprintnum(int x,int y,int64 val);
        void kprinthex(int x,int y,int64 val);

        extern void kprintnum2(int64 x);


#endif
