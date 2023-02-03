#pragma once

#include <cdefs.h>




__BEGIN_DECLS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


void* malloc(size_t size);

void* realloc(void* mem, size_t new_size);

void free(void* ptr);

char* ltoa(int64 value, char* str, int base);

char* ultoa(uint64 value, char* str, int base);

char* utoa(uint64 value, char* str, int base);

char* itoa(int value, char* str, int base);

int64 strtol(const char* str, char** endptr, int base);

uint64 strtoul(const char* str, char** endptr, int base);

uint64 atol(const char* str);

int atoi(const char* str);

#ifndef BUILD_DISABLE_FPA
void ftoa(float value, char* str, int precision);

double pow(double x, double y);

#endif

int abs(int i);


int sqrt(int x);

__END_DECLS
