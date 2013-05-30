#include <errno.h>

#undef errno
extern int errno;

int times(int buf) {
    errno = ENOSYS;
    return -1;
}
