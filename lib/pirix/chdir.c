#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

int chdir(const char* path) {
    errno = ENOENT;
    return -1;
}
