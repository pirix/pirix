#include <sys/pirix.h>

int getpid() {
    return sys_getpid();
}
