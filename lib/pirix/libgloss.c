#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <pirix.h>
#include <pirix/ipc.h>
#include <vfs/vfs.h>

#undef errno
extern int errno;

void exit(int status) {
    sys_exit(status);
    while (1);
}

int execve(char* name, char** argv, char** env) {
    return sys_execve(name, argv, env);
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

int isatty(int file) {
    return sys_isatty(file);
}

int kill(int pid, int sig) {
    return sys_kill(pid, sig);
}

int sbrk(int incr) {
    return sys_sbrk(incr);
}

int open(char* name, int flags, ...) {
    int fd = sys_connect(VFS_PID);

    message msg;
    msg.tag = VFS_OPEN;

    int res = sys_send(fd, &msg);

    return res < 0 ? res : fd;
}

int read(int file, char* buf, int len) {
    return sys_read(file, buf, len);
}

int write(int file, char* buf, int len) {
    return sys_write(file, buf, len);
}

int close(int file) {
    return sys_close(file);
}

int link(char* old, char* new) {
    return sys_link(old, new);
}

int unlink(char* name) {
    return sys_unlink(name);
}

int stat(const char* path, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int fstat(int file, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int lseek(int file, int offset, int whence) {
    return sys_lseek(file, offset, whence);
}

int times(int buf) {
    errno = ENOSYS;
    return -1;
}

int gettimeofday(struct timeval* p, void* z){
    return -1;
}
