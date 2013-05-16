#include <stdio.h>
#include <pirix.h>
#include <pirix/ipc.h>
#include <vfs/vfs.h>
#include <sys/utsname.h>

int main(int argc, const char* argv[]) {
    struct utsname name;
    uname(&name);

    printf("%s %s (%s)\n\n", name.sysname, name.version, name.machine);

    printf("Login: ");
    fflush(stdout);

    while (1);
    return 0;
}
