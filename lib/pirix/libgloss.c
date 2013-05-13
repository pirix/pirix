#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <pirix.h>
#include <pirix/ipc.h>
#include <vfs/vfs.h>

#undef errno
extern int errno;

char* __env[1] = { 0 };
char** environ = __env;

void exit(int status) {
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
    return -1;
    int fd = sys_connect(VFS_PID);

    message msg;
    msg.tag = VFS_OPEN;

    int res = sys_send(fd, &msg);

    return res < 0 ? res : fd;
}

int read(int fd, char* buf, int len) {
    return 0;
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
