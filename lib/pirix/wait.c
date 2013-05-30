#include <sys/types.h>
#include <sys/pirix.h>

int wait(int* status) {
    return sys_wait(status);
}

pid_t waitpid(pid_t pid, int *stat_loc, int options) {
    return 0;
}
