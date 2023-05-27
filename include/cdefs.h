#ifndef __CDEFS_H
#define __CDEFS_H

/* This is not a typedef so `const __ptr_t' does the right thing.  */
#define __ptr_t void *


/* C++ needs to know that types and declarations are C, not C++.  */
#ifdef	__cplusplus
# define __BEGIN_DECLS	extern "C" {
# define __END_DECLS	}
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

#endif