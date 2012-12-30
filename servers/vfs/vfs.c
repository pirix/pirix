#include "pirix.h"
#include "pirix/ipc.h"
#include "vfs.h"
#include "vnode.h"
#include <stdio.h>

void handle(message* msg) {
    switch (msg->tag) {
    case VFS_OPEN:
        break;
    case VFS_CLOSE:
        break;
    case VFS_MOUNT:
        break;
    case VFS_UMOUNT:
        break;
    }
}

int main(int argc, char* argv[]) {
    sys_log("virtual file system started.");
    vnode_init(100);

    message msg;

    while (1) {
        sys_recv(&msg);
        handle(&msg);
    }

    return 0;
}
