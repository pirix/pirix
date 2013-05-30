#include <sys/pirix.h>

int wait(int* status) {
    return sys_wait(status);
}
