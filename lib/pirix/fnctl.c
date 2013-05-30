#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

int fcntl(int fd, int cmd, ...) {
    errno = EACCES;
    return -1;
}
