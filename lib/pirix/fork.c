#include <sys/pirix.h>

int fork() {
    return sys_fork();
}
