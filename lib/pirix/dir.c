#include <sys/stat.h>
#include <errno.h>

#undef errno
extern int errno;

int mkdir(const char* path, mode_t mode) {
    errno = ENOENT;
    return -1;
}

int rmdir(const char* path) {
    errno = ENOENT;
    return -1;
}
