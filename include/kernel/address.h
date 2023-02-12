#pragma once

#include <stdvar.h>

#define L4_ADDR 0x1000

#define KERNEL_OFFSET ((uint64)0xFFFFFF8000000000)
#define PAGINATION_TEMP_OFFSET ((uint64)0xFFFFFFFFC0000000)

#define BASE_KERNEL_ADDRESS ((uint64)KERNEL_OFFSET+0x7E00)
#define VGA_ADDRESS ((uint64)KERNEL_OFFSET + 0xb8000)

