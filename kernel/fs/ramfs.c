#include <pirix/fs.h>
#include <pirix/kheap.h>

unsigned ramfs_read(fs_node* node, unsigned off, unsigned len, char* buff) {
    return 0;
}

unsigned ramfs_write(fs_node* node, unsigned off, unsigned len, const char* buff) {
    return 0;
}

static fs_node_ops ramfs_node_ops = {
    .read = &ramfs_read,
    .write = &ramfs_write,
};

fs_superblock* ramfs_mount(void* args) {
    fs_superblock* sb = kmalloc(sizeof(fs_superblock));

    fs_node* root = kmalloc(sizeof(fs_node));
    root->flags = FS_DIRECTORY;
    root->ops = &ramfs_node_ops;

    sb->root = root;
    return sb;
}

static fs_type ramfs_type = {
    .name = "ramfs",
    .mount = &ramfs_mount
};

void ramfs_init() {
    fs_type_register(&ramfs_type);
}
