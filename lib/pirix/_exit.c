#include <sys/pirix.h>

void _exit(int status) {
    sys_exit(status);
    while (1);
}
