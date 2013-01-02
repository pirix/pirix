#include "vfs/vfs.h"
#include "pirix/ipc.h"
#include "pirix.h"

void devfs_register() {
    message msg = (message) {
        .tag = VFS_MOUNT,
        .data = 23
    };
    sys_call(0, &msg);
}

void devfs_run() {
    devfs_register();

    message msg;
    while (1) {
        sys_recv(&msg);
    }
}
