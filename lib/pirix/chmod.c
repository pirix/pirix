#include <sys/stat.h>
#include <errno.h>

#undef errno
extern int errno;

int fchmod(int fd, mode_t mode) {
    errno = ENOENT;
    return -1;
}

int chmod(const char* path, mode_t mode) {
    errno = ENOENT;
    return -1;
}
