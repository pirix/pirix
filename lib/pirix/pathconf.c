#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

long fpathconf(int fd, int name) {
    errno = ENOENT;
    return -1;
}

long pathconf(const char *path, int name) {
    errno = ENOENT;
    return -1;
}
