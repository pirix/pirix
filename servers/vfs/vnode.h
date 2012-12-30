#pragma once

typedef struct vnode {
    unsigned fs_pid;
    unsigned inode;
} vnode;

void vnode_init(unsigned size);
vnode* vnode_get_free();
