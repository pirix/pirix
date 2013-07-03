#pragma once

#define BOOT_ARGS_MAX 50

typedef struct boot_info {
    char args[BOOT_ARGS_MAX];
} boot_info;

void modules_init();
