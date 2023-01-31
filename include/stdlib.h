#ifndef STDLIB_H
#define STDLIB_H

#include <stdvar.h>

#ifdef __cplusplus
extern "C" {
#endif


void     *malloc(size_t);				//< The standard function.
void     *realloc(void *, size_t);		//< The standard function.
void     *calloc(size_t, size_t);		//< The standard function.
void      free(void *);					//< The standard function.


#ifdef __cplusplus
}
#endif

#endif