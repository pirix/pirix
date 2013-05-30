#include <unistd.h>
#include <errno.h>

#undef errno
extern int errno;

char* getcwd(char* buf, size_t size) {
    errno = EACCES;
    return 0;
}
