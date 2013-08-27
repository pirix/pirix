#pragma once

#include <pirix/types.h>

#define BOOT_ARGS_MAX 100
#define BOOT_MOD_MAX 15
#define BOOT_MOD_ARGS_MAX 30

typedef struct boot_module {
    uintptr_t addr;
    size_t size;
    char args[BOOT_MOD_ARGS_MAX];
} boot_module;

typedef struct boot_info {
    char args[BOOT_ARGS_MAX];
    boot_module modules[BOOT_MOD_MAX];
    size_t mod_count;
} boot_info;