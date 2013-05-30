#include <errno.h>

#undef errno
extern int errno;

int execve(const char* path, char* const argv[], char* const env[]) {
    errno = ENOMEM;
    return -1;
}

int execv(const char* path, char* const argv[]) {
    return execve(path, argv, 0);
}

int execvp(const char* file, char* const argv[]) {
    return execve(file, argv, 0);
}
