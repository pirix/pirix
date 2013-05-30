#include <sys/pirix.h>

int kill(int pid, int sig) {
    return sys_kill(pid, sig);
}
