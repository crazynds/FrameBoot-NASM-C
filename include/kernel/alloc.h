#pragma once


void* kernel_calloc(size_t size);
void* kernel_malloc(size_t size);
void* kernel_realloc(void* mem, size_t new_size);
void kernel_free(void* ptr);