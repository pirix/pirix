#include <sys/stat.h>

int fstat(int fd, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int stat(const char* path, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int lstat(const char* path, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}
