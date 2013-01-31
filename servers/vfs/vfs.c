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

void handle(int cid, message* msg) {
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

    message rmsg;
    rmsg.tag = 123;
    sys_reply(cid, &rmsg);
}

int main(int argc, char* argv[]) {
    sys_log("virtual file system started.");
    vnode_init_cache(100);

    sys_listen();

    while (1) {
        message msg;
        int cid = sys_recv(&msg);
        handle(cid, &msg);
    }

    return 0;
}
