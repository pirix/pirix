#include <ix/fs.h>

fs_node* fs_root;

unsigned fs_read(fs_node* node, unsigned offset, unsigned size, char* buffer) {
    if (node->read) {
        return node->read(node, offset, size, buffer);
    }
    else {
        return 0;
    }
}

unsigned fs_write(fs_node* node, unsigned offset, unsigned size, char* buffer) {
    if (node->write) {
        return node->write(node, offset, size, buffer);
    }
    else {
        return 0;
    }
}

void fs_open(fs_node* node, char read, char write) {
    if (node->open) {
        node->open(node);
    }
}

void fs_close(fs_node* node) {
    if (node->close) {
        node->close(node);
    }
}

dirent* fs_readdir(fs_node* node, unsigned index) {
    if (node->readdir) {
      return node->readdir(node, index);
    }
    else {
        return 0;
    }
}

fs_node* fs_finddir(fs_node* node, char* name) {
    if (node->finddir) {
        return node->finddir(node, name);
    }
    else {
        return 0;
    }
}
