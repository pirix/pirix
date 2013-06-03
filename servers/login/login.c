#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <servers/system.h>

int main(int argc, const char* argv[]) {
    pirix_syscall(SYS_LOG, "hello!", 0, 0, 0);

    puts("Hello\n");

    while (1);

    //int fd = open("/etc/passwd", O_RDONLY);

    //printf("fd: %i\n", fd);

    while (1);

    return 0;
}
