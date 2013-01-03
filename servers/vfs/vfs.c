#include <pirix.h>
#include <pirix/ipc.h>
#include <stdio.h>
#include "vfs.h"
#include "vnode.h"

int vfs_mount(unsigned point, unsigned pid) {
    sys_log("mount!");

    vnode* v = vnode_get_free();
    v->fs_pid = pid;
    v->inode = point;

    return 0;
}

void handle(message* msg) {
    switch (msg->tag) {
    case VFS_OPEN:
        break;
    case VFS_CLOSE:
        break;
    case VFS_MOUNT:
        vfs_mount(1, msg->src);
        break;
    case VFS_UMOUNT:
        break;
    }
}

int main(int argc, char* argv[]) {
    sys_log("virtual file system started.");
    vnode_init_cache(100);

    message msg;

    while (1) {
        sys_recv(ANY_PID, &msg);
        handle(&msg);
    }

    return 0;
}
