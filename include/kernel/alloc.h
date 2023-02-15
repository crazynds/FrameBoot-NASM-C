#pragma once

#include <cdefs.h>

__BEGIN_DECLS

void* kernel_calloc(size_t nobj,size_t size);
void* kernel_malloc(size_t size);
void* kernel_realloc(void* mem, size_t new_size);
void kernel_free(void* ptr);

__END_DECLS