#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#undef errno
extern int errno;

int fchown(int fd, uid_t owner, gid_t group) {
    errno = ENOENT;
    return -1;
}

int chown(const char* path, uid_t owner, gid_t group) {
    errno = ENOENT;
    return -1;
}

int lchown(const char* path, uid_t owner, gid_t group) {
    errno = ENOENT;
    return -1;
}
