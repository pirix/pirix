#include <stdio.h>
#include <errno.h>

#undef errno
extern int errno;

FILE* popen(const char* command, const char* mode) {
    errno = EMFILE;
    return 0;
}

int pclose(FILE *stream) {
    errno = ECHILD;
    return -1;
}
