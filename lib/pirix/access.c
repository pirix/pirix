#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

int access(const char* path, int amode) {
    errno = ENOENT;
    return -1;
}
