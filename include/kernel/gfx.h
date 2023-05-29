#ifndef GFX_H
    #define GFX_H

        #include <stdvar.h>

        #define CLEAR_TEXT_COLOR(P) (P&0xF0)
        #define CLEAR_BACKGROUND_COLOR(P) (P&0x0F)

        enum {
            TEXT_BLACK         = 0,
            TEXT_BLUE          = 1,
            TEXT_GREEN         = 2,
            TEXT_CYAN          = 3,
            TEXT_RED           = 4,
            TEXT_MAGENTA       = 5,
            TEXT_BROW          = 6,
            TEXT_LIGHT_GRAY    = 7,
            TEXT_DARK_GRAY     = 8,
            TEXT_LIGHT_BLUE    = 9,
            TEXT_LIGHT_GREEN   = 10,
            TEXT_LIGHT_CYAN    = 11,
            TEXT_LIGHT_RED     = 12,
            TEXT_PINK          = 13,
            TEXT_YELLOW        = 14,
            TEXT_WHITE         = 15,
        };
        enum {
            BACKGROUND_BLACK         = 0 << 4,
            BACKGROUND_BLUE          = 1 << 4,
            BACKGROUND_GREEN         = 2 << 4,
            BACKGROUND_CYAN          = 3 << 4,
            BACKGROUND_RED           = 4 << 4,
            BACKGROUND_MAGENTA       = 5 << 4,
            BACKGROUND_BROW          = 6 << 4,
            BACKGROUND_LIGHT_GRAY    = 7 << 4,
            BACKGROUND_DARK_GRAY     = 8 << 4,
            BACKGROUND_LIGHT_BLUE    = 9 << 4,
            BACKGROUND_LIGHT_GREEN   = 10 << 4,
            BACKGROUND_LIGHT_CYAN    = 11 << 4,
            BACKGROUND_LIGHT_RED     = 12 << 4,
            BACKGROUND_PINK          = 13 << 4,
            BACKGROUND_YELLOW        = 14 << 4,
            BACKGROUND_WHITE         = 15 << 4,
        };

        typedef int8 color;


        //Utilitários para kprintnum
        void int_asc(int64 num, char *numero);
        int gettam_num(int64 num);
        uint64 asc_int(char *numero);

        void clrscr(color c);
        extern "C" void kprintChar(int x,int y,char ch,color c);
        extern "C" void kprintStr(int x,int y,const char *s,color c);
        void kprintnum(int x,int y,int64 val);
        void kprinthex(int x,int y,uint64 val);

        void disableCursor();
        void enableCursor(uint8 cursor_start,uint8 cursor_end);
        void setCursorPosition(uint16 position);


#endif
