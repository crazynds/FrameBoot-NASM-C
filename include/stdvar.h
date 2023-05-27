#ifndef VAR_H
    #define VAR_H

    #include <stdbool.h>

    #ifndef NULL
	    #define NULL 0L
	#endif
    
    typedef unsigned long long int uint64;
    typedef unsigned int uint32;
    typedef unsigned short uint16;
    typedef unsigned char uint8;
    typedef long long int int64;
    typedef int int32;
    typedef short int16;
    typedef char int8;

    typedef unsigned long int  size_t;

    typedef void* ptr_t;



    typedef char boolean;

    #define KB ((uint64)1024)
    #define MB ((uint64)1024*KB)
    #define GB ((uint64)1024*MB)
#endif