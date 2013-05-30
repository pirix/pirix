#include <sys/pirix.h>

int isatty(int fd) {
    return (fd == 1) ? 1 : 0;
}
