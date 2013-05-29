#include <fcntl.h>
#include <stdio.h>
#include <pirix/ipc.h>
#include <sys/pirix.h>
#include <servers/system.h>
#include <sys/utsname.h>
#include <sys/uio.h>

int main(int argc, const char* argv[]) {
    struct utsname name;
    uname(&name);

    printf("%s %s (%s)\n\n", name.sysname, name.version, name.machine);

    printf("Login: ");
    fflush(stdout);

    int fd = open("/etc/passwd", O_RDONLY);

    printf("fd: %i\n", fd);

    while (1);

    return 0;
}
