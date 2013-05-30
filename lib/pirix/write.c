#include <sys/pirix.h>

int write(int fd, char* buf, int len) {
    if (fd == 1) sys_print(buf, len);
    return len;
}
