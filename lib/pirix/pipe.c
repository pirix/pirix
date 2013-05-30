#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

int pipe(int fds[2]) {
    errno = ENFILE;
    return -1;
}
