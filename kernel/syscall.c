#include <ix/syscall.h>
#include <ix/task.h>
#include <ix/kprint.h>

typedef int stat;
typedef int clock;
typedef int tms;
typedef int timeval;
typedef int timezone;

static int sys_reboot() {
    return 0;
}

static int sys_log(char* str) {
    kputs(str);
    return 0;
}

static int sys_exit(int retval) {
    task_exit(retval);
    while (1);
    return 0;
}

static int sys_kill(int pid, int sig) {
    return 0;
}

static int sys_wait(int* status) {
    return 0;
}

static int sys_fork() {
    return 0;
}

static int sys_getpid() {
    return 0;
}

static int sys_isatty(int fd) {
    return 0;
}

static int sys_link(char* old, char* new) {
    return 0;
}

static int sys_unlink(char* name) {
    return 0;
}

static int sys_execve(char* name, char** argv, char** env) {
    return 0;
}

static int sys_close(int fd) {
    return 0;
}

static int sys_open(const char* name, int flags, int mode) {
    return 0;
}

static int sys_read(int fd, char* ptr, int len) {
    return 0;
}

static int sys_write(int fd, char* ptr, int len) {
    return 0;
}

static int sys_lseek(int fd, char* ptr, int dir) {
    return 0;
}

static int sys_stat(const char* file, stat* st) {
    return 0;
}

static int sys_fstat(int file, stat* st) {
    return 0;
}

int syscall(int a, int b, int c, int d, int id) {
    switch (id) {
    case SYS_REBOOT:
        return sys_reboot();

    case SYS_LOG:
        return sys_log((char*)a);

    case SYS_EXIT:
        return sys_exit(a);

    case SYS_KILL:
        return sys_kill(a, b);

    case SYS_WAIT:
        return sys_wait((int*)a);

    case SYS_FORK:
        return sys_fork();

    case SYS_GETPID:
        return sys_getpid();

    case SYS_ISATTY:
        return sys_isatty(a);

    case SYS_EXECVE:
        return sys_execve((char*)a, (char**)b, (char**)c);

    case SYS_LINK:
        return sys_link((char*)a, (char*)b);

    case SYS_UNLINK:
        return sys_unlink((char*)a);

    case SYS_CLOSE:
        return sys_close(a);

    case SYS_OPEN:
        return sys_open((const char*)a, b, c);

    case SYS_READ:
        return sys_read(a, (char*)b, c);

    case SYS_WRITE:
        return sys_write(a, (char*)b, c);

    case SYS_LSEEK:
        return sys_lseek(a, (char*)b, c);

    case SYS_STAT:
        return sys_stat((const char*)a, (stat*)b);

    case SYS_FSTAT:
        return sys_fstat(a, (stat*)b);

    default:
        return 0;
    }
}
