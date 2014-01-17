#pragma once

#include <arch/types.h>

#define PAGE_PERM_KRNL 0x0
#define PAGE_PERM_USER 0x4

#define PAGE_SIZE 0x1000

typedef uint32_t* pagedir;
