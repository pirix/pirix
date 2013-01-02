#pragma once

typedef struct vnode {
    unsigned fs_pid;
    unsigned inode;
} vnode;

void vnode_init_cache(unsigned size);
vnode* vnode_get_free();
