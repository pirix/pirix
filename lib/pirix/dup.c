#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

int dup(int fd) {
    errno = EBADF;
    return -1;
}

int dup2(int fd, int fd2) {
    errno = EBADF;
    return -1;
}
