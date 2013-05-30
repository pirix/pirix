#include <sys/pirix.h>

int sbrk(int incr) {
    return sys_sbrk(incr);
}

int _sbrk(int incr) {
    return sys_sbrk(incr);
}
