#include <pirix/initrd.h>
#include <pirix/kheap.h>
#include <pirix/fs.h>
#include <tar.h>
#include <string.h>

typedef struct initrd_file {
    char* name;
    char* data;
} initrd_file;

static fs_node* initrd_nodes;
static initrd_file* initrd_files;
static unsigned initrd_file_count;
static dirent dir_entry;

unsigned initrd_read(fs_node* node, unsigned offset, unsigned size, char* buffer) {
    char* data = initrd_files[node->id].data;

    if (offset > node->size) return 0;

    if (offset + size > node->size) {
        size = node->size - offset;
    }

    memcpy(buffer, data, size);
    return size;
}

dirent* initrd_dir_read(fs_node* node, unsigned index) {
    if (index > initrd_file_count) return 0;

    strcpy(dir_entry.name, initrd_files[index].name);
    dir_entry.node = 0;

    return &dir_entry;
}

fs_node* initrd_dir_find(fs_node* node, const char* name) {
    for (unsigned i = 0; i < initrd_file_count; i++) {
        if (!strcmp(name, initrd_files[i].name)) {
            return initrd_nodes+i;
        }
    }
    return 0;
}

static fs_node_ops initrd_node_ops = {
    &initrd_read,
    0, 0, 0,
    &initrd_dir_read,
    &initrd_dir_find
};

static long read_octal(char* ptr, int length) {
    long result = 0;
    int padding = 1;

    for (int i = 0; i < length; i++) {
        if (ptr[i] == 0) break;
        if (ptr[i] == '0' || ptr[i] == ' ') {
            if (padding) continue;
            if (ptr[i] ==  ' ') break;
        }
        padding = 0;
        result = (result << 3) + (ptr[i] - '0');
    }

    return result;
}

static int initrd_count_files(tar_header* header) {
    int count = 0;
    while (header->filename[0]) {
        header += read_octal(header->size, 12);
        header += TAR_HEADER_SIZE;
        count++;
    }
    return count;
}

fs_superblock* initrd_get_sb(void* flags) {
    // create root directory
    fs_node* root = kcalloc(1, sizeof(fs_node));
    root->flags = FS_DIRECTORY;
    root->ops = &initrd_node_ops;

    extern tar_header _binary_initrd_start;
    tar_header* header = &_binary_initrd_start;

    // allocate space
    initrd_file_count = initrd_count_files(header);
    initrd_nodes = kcalloc(initrd_file_count, sizeof(fs_node));
    initrd_files = kmalloc(sizeof(initrd_file) * initrd_file_count);

    for (unsigned i = 0; i < initrd_file_count; i++) {
        fs_node* node = initrd_nodes + i;

        initrd_files[i].data = (char*)((unsigned)header + TAR_HEADER_SIZE);
        initrd_files[i].name = header->filename;

        node->id = i;
        node->size = read_octal(header->size, 12);
        node->flags = FS_FILE;
        node->ops = &initrd_node_ops;

        header += TAR_HEADER_SIZE + node->size;
    }

    fs_superblock* superblock = kcalloc(1, sizeof(fs_superblock));
    superblock->root = root;

    return superblock;
}

int initrd_init() {
    fs_register_type(&(fs_type){"initrd", &initrd_get_sb});
    fs_mount("initrd", 0, "/");
    return 0;
}
