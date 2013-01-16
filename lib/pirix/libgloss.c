#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include "pirix.h"

#undef errno
extern int errno;

void _exit(int status) {
    sys_exit(status);
    while (1);
}

int _execve(char* name, char** argv, char** env) {
    return sys_execve(name, argv, env);
}

int _fork() {
    return sys_fork();
}

int _wait(int* status) {
    return sys_wait(status);
}

int _getpid() {
    return sys_getpid();
}

int _isatty(int file) {
    return sys_isatty(file);
}

int _kill(int pid, int sig) {
    return sys_kill(pid, sig);
}

int _sbrk(int incr) {
    return sys_sbrk(incr);
}

int _open(char* name, int flags, ...) {
    return sys_open(name, flags);
}

int _read(int file, char* buf, int len) {
    return sys_read(file, buf, len);
}

int _write(int file, char* buf, int len) {
    return sys_write(file, buf, len);
}

int _close(int file) {
    return sys_close(file);
}

int _link(char* old, char* new) {
    return sys_link(old, new);
}

int _unlink(char* name) {
    return sys_unlink(name);
}

int _stat(int file, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _fstat(int file, struct stat* st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _lseek(int file, int offset, int whence) {
    return sys_lseek(file, offset, whence);
}

int _times(int buf) {
    errno = ENOSYS;
    return -1;
}

int _gettimeofday(struct timeval* p, void* z){
    return -1;
}
