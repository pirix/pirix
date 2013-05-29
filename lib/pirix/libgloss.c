#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <sys/pirix.h>
#include <sys/uio.h>
#include <pirix/ipc.h>
#include <servers/system.h>

#undef errno
extern int errno;

void _exit(int status) {
    sys_exit(status);
    while (1);
}

int execve(char* name, char** argv, char** env) {
    errno = ENOMEM;
    return -1;
}

int fork() {
    return sys_fork();
}

int wait(int* status) {
    return sys_wait(status);
}

int getpid() {
    return sys_getpid();
}

int isatty(int fd) {
    return (fd == 1) ? 1 : 0;
}

int kill(int pid, int sig) {
    return sys_kill(pid, sig);
}

int sbrk(int incr) {
    return sys_sbrk(incr);
}

int _sbrk(int incr) {
    return sys_sbrk(incr);
}

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

int read(int fd, char* buf, int len) {
    //io_read_msg msg;

    //msg.type = IO_READ;
    //msg.size = len;
    const char msg[] = "READ";

    msg_buffer out = MSG_BUF_LIN(msg, sizeof(msg));
    msg_buffer in = MSG_BUF_LIN(buf, len);

    return ipc_send(fd, &out, &in);
}

int write(int fd, char* buf, int len) {
    if (fd == 1) sys_print(buf, len);
    return len;
}

int close(int fd) {
    return -1;
}

int link(char* old, char* new) {
    errno = EMLINK;
    return -1;
}

int unlink(char* name) {
    errno = ENOENT;
    return -1;
}

int stat(const char* path, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int fstat(int fd, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int lseek(int fd, int offset, int whence) {
    return 0;
}

int times(int buf) {
    errno = ENOSYS;
    return -1;
}

int gettimeofday(struct timeval* p, void* z){
    return -1;
}
