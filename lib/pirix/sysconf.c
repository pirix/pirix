#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

long sysconf(int name) {
    errno = EINVAL;
    return -1;
}
