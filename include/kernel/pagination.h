#ifndef PAGINATION_H
    #define PAGINATION_H

    #include <stdvar.h>

    #define L4 1
    #define L3 512          //all L3 in L4 map 512GB
    #define L2 262144       //all L2 in L3 map 1GB
    #define L1 134217728    //all L1 in L2 map 2MB

    #define RW 1<<1
    #define PRESENT 1<<0
    #define USER 1<<2
    #define WRITE_THROUGH 1<<3
    #define PAGE_4MB_SIZED 1<<7

    #define REAL_PAG_MEM 0x100000



    const char* FAULT[8] = {
        "Supervisory process tried to read a non-present page entry",
        "Supervisory process tried to read a page and caused a protection fault",
        "Supervisory process tried to write to a non-present page entry",
        "Supervisory process tried to write a page and caused a protection fault",
        "User process tried to read a non-present page entry",
        "User process tried to read a page and caused a protection fault",
        "User process tried to write to a non-present page entry",
        "User process tried to write a page and caused a protection fault",
    };

    struct memory_map{
        uint64 base;
        uint64 size;
        uint32 type;
        uint32 extendedAtb;
    }__attribute__((packed));



    class PaginationTable{
    private:
        uint64* l4;
    public:
        // Paginate 4kb memory
        void setPage(uint64 *virtualPage,uint64 *fisicalFrame,uint16 flags);

        // Paginate 2mb memory
        void setPageFlags(uint64 *virtualPage,uint16 flags);

        void setDirectoryPage(uint64 *virtualPage,uint64 *fisicalFrame,uint16 flags);
        void setDirectoryFlags(uint64 *virtualPage,uint16 flags);
    };

#endif
