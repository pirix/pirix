#pragma once

#define BOOT_MAGIC 0x427d70e4
#define BOOT_MAX_MODULES 10
#define BOOT_MAX_MODNAME 15

typedef struct boot_module {
    char name[15];
    unsigned addr;
    unsigned size;
    unsigned entry;
} boot_module;

typedef struct boot_header {
    unsigned magic;
    unsigned module_count;
    boot_module modules[BOOT_MAX_MODULES];
} boot_header;
