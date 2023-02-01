#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void* calloc(size_t size)
{
    void* mem = malloc(size);
    memset(mem, 0, size);

    return mem;
}

template <typename T>
static char* int_to_string(T value, char* str, int base)
{
    char* rc;
    char* ptr;
    char* low;

    if(base < 2 || base > 36)
    {
        *str = '\0';

        return str;
    }
    rc = ptr = str;

    if(value < 0 && base == 10)
    {
        *ptr++ = '-';
    }

    low = ptr;
    do
    {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    }
    while(value);

    *ptr-- = '\0';

    while(low < ptr)
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }

    return rc;
}

char* ltoa(int64 value, char* str, int base)
{
    return int_to_string<int64>(value, str, base);
}

char* ultoa(uint64 value, char* str, int base)
{
    return int_to_string<uint64>(value, str, base);
}

char* utoa(uint64 value, char* str, int base)
{
    return int_to_string<uint64>(value, str, base);
}

char* itoa(int value, char* str, int base)
{
    return int_to_string<int>(value, str, base);
}

template <typename T>
T string_to_int(const char* str, char** endptr, int base)
{
    T res = 0;
    size_t len = strlen(str);
    size_t i = (str[0] == '-') || (str[0] == '+');

    *endptr = (char*)str + len;

    if(base < 1)
    {
        base = 10;
    }

    for(; i < len; i++)
    {
        if(str[i] > (('0' - 1) + base) || (str[i] < '0' && endptr != NULL))
        {
            *endptr = (char*)str + i;

            break;
        }

        res = (res * base) + (str[i] - '0');
    }

    if(str[0] == '-')
    {
        res = -res;
    }

    return res;
}

int64 strtol(const char* str, char** endptr, int base)
{
    return string_to_int<int64>(str, endptr, base);
}

uint64 strtoul(const char* str, char** endptr, int base)
{
    return string_to_int<uint64>(str, endptr, base);
}

uint64 atol(const char* str)
{
    return string_to_int<uint64>(str, nullptr, 10);
}

int atoi(const char* str)
{
    return string_to_int<int>(str, nullptr, 10);
}

#ifndef BUILD_DISABLE_FPA

void ftoa(float, char*, int)
{

}

double pow(double x, double y)
{
    double result = x;

    for(double d = 0.0f; d < y; d++)
    {
        result *= x;
    }

    return result;
}

#endif

int abs(int i)
{
    return i < 0 ? -i : i;
}

int sqrt(int x)
{
    int quotient = 0;
    int i = 0;

    bool answer_found = false;
    while(answer_found == false)
    {
        if(i * i == x)
        {
          quotient = i;
          answer_found = true;
        }

        i++;
    }

    return quotient;
}