#include <pirix.h>
#include <pirix/kheap.h>
#include <pirix/fs.h>
#include <string.h>

#define FS_TYPES_MAX 10
static fs_type* fs_types[FS_TYPES_MAX];

fs_node* fs_root;

int fs_init() {
    extern void ramfs_init();
    ramfs_init();

    fs_mount("ramfs", 0, "/");

    return 0;
}

void fs_type_register(fs_type* type) {
    int i = 0;
    while (fs_types[i] && i < FS_TYPES_MAX) i++;
    if (i < FS_TYPES_MAX) {
        fs_types[i] = type;
    }
}

fs_type* fs_type_find(const char* name) {
    for (int i = 0; i < FS_TYPES_MAX; i++) {
        if (fs_types[i] && !strcmp(fs_types[i]->name, name)) {
            return fs_types[i];
        }
    }
    return NULL;
}

void fs_mount(const char* type, void* flags, const char* path) {
    fs_type* t = fs_type_find(type);
    fs_superblock* sb = t->mount(flags);
    sb->type = t;

    if (!strcmp(path, "/")) {
        fs_root = sb->root;
    }
}

fs_node* fs_lookup(const char* path) {
    fs_node* node = NULL;

    if (path[0] == '/') {
        node = fs_root;
    }
    else {
        // node = current working directory
    }

    if (!node) return node;

    char* fpath = strdup(path);
    char* part;
    char** save = NULL;
    part = strtok_r(fpath, "/", save);

    do {
        //node = fs_dir_find(node, part);
        if (!node) break;
    } while ((part = strtok_r(0, "/", save)));

    kfree(fpath, sizeof(char)*strlen(path));
    return node;
}

unsigned fs_read(fs_node* node, unsigned off, unsigned len, char* buff) {
    if (node->ops && node->ops->read) {
        return node->ops->read(node, off, len, buff);
    }
    else return 0;
}

unsigned fs_write(fs_node* node, unsigned off, unsigned len, const char* buff) {
    if (node->ops && node->ops->write) {
        return node->ops->write(node, off, len, buff);
    }
    else return 0;
}
