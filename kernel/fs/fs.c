#include <pirix.h>
#include <pirix/kheap.h>
#include <pirix/fs.h>
#include <string.h>

#define FS_TYPES_MAX 10
static fs_type* fs_types[FS_TYPES_MAX];

fs_node* fs_root;

int fs_init() {
    //fs_root = kcalloc(1, sizeof(fs_node));
    //fs_root->flags = FS_DIRECTORY | FS_MOUNTPOINT;
    return 0;
}

void fs_register_type(fs_type* type) {
    int i = 0;
    while (fs_types[i] && i < FS_TYPES_MAX) i++;
    if (i < FS_TYPES_MAX) {
        fs_types[i] = type;
    }
}

fs_type* fs_find_type(const char* name) {
    for (int i = 0; i < FS_TYPES_MAX; i++) {
        if (fs_types[i] && !strcmp(fs_types[i]->name, name)) {
            return fs_types[i];
        }
    }
    return 0;
}

void fs_mount(const char* type, void* flags, const char* path) {
    fs_type* t = fs_find_type(type);
    fs_superblock* sb = t->get_sb(flags);
    fs_root = sb->root;
}

fs_node* fs_lookup(const char* path) {
    if (!strcmp(path, "/")) {
        return fs_root;
    }

    return 0;
}

unsigned fs_read(fs_node* node, unsigned offset, unsigned size, char* buffer) {
    if (node->ops->read) {
        return node->ops->read(node, offset, size, buffer);
    }
    else return 0;
}

unsigned fs_write(fs_node* node, unsigned offset, unsigned size, char* buffer) {
    if (node->ops->write) {
        return node->ops->write(node, offset, size, buffer);
    }
    else return 0;
}

void fs_open(fs_node* node, char read, char write) {
    if (node->ops->open) {
        node->ops->open(node, read, write);
    }
}

void fs_close(fs_node* node) {
    if (node->ops->close) {
        node->ops->close(node);
    }
}

dirent* fs_dir_read(fs_node* node, unsigned index) {
    if ((node->flags & FS_DIRECTORY) && node->ops->dir_read) {
        return node->ops->dir_read(node, index);
    }
    else return 0;
}

fs_node* fs_dir_find(fs_node* node, const char* name) {
    if ((node->flags & FS_DIRECTORY) && node->ops->dir_find) {
        return node->ops->dir_find(node, name);
    }
    else return 0;
}
