#include <pirix/ipc.h>
#include <servers/system.h>
#include <sys/uio.h>
#include <string.h>
#include <errno.h>

#undef errno
extern int errno;

int open(char* name, int flags, ...) {
    int fd = ipc_connect(0, 0);

    if (fd < 0) {
        errno = ESRCH;
        return -1;
    }

    struct pathmgr_msg msg;
    msg.tag = PATHMGR_RESOLVE;

    struct iovec iov[2];
    SETIOV(&iov[0], &msg, sizeof(msg));
    SETIOV(&iov[1], name, strlen(name));

    return ipc_send(fd, &MSG_BUF_VEC(iov, 2), NULL);
}
