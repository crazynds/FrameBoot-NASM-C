#ifndef GFX_H
    #define GFX_H

        #include <stdvar.h>

        enum {
            COLOR_BLACK         = 0,
            COLOR_BLUE          = 1,
            COLOR_GREEN         = 2,
            COLOR_CYAN          = 3,
            COLOR_RED           = 4,
            COLOR_MAGENTA       = 5,
            COLOR_BROW          = 6,
            COLOR_LIGHT_GRAY    = 7,
            COLOR_DARK_GRAY     = 8,
            COLOR_LIGHT_BLUE    = 9,
            COLOR_LIGHT_GREEN   = 10,
            COLOR_LIGHT_CYAN    = 11,
            COLOR_LIGHT_RED     = 12,
            COLOR_PINK          = 13,
            COLOR_YELLOW        = 14,
            COLOR_WHITE         = 15,
        };



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
