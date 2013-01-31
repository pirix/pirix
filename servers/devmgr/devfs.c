#include "vfs/vfs.h"
#include <fcntl.h>
#include <pirix/ipc.h>
#include <pirix.h>

void devfs_register() {
}

void devfs_run() {
    int fd = open("/dev/null", 0);
    sys_log("opened!");
    while (1);
}
