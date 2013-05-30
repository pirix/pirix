#pragma once

#include <sys/types.h>

typedef struct dirent {
    ino_t d_ino;
    char d_name[256];
} dirent;

typedef struct DIR {
    int fd;
} DIR;

int closedir(DIR* dir);
DIR* opendir(const char* path);
struct dirent* readdir(DIR* dir);
