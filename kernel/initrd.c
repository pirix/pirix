#include <pirix/initrd.h>
#include <pirix/kheap.h>
#include <tar.h>
#include <string.h>

extern tar_header _binary_initrd_start;
static tar_header* initrd_header = &_binary_initrd_start;

static fs_node*  initrd_root;
static fs_node*  initrd_nodes;
static unsigned* initrd_files;
static int       initrd_file_count;

static unsigned initrd_read(fs_node* node, unsigned offset, unsigned size, char* buffer) {
    char* data = (char*)initrd_files[node->inode];

    if (offset > node->length) return 0;

    if (offset+size > node->length) {
        size = node->length - offset;
    }

    memcpy(buffer, data, size);
    return size;
}

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

static int initrd_count_files() {
    int count = 0;
    tar_header* header = initrd_header;
    while (header->filename[0]) {
        header += read_octal(header->size, 12);
        header += TAR_HEADER_SIZE;
        count++;
    }
    return count;
}

fs_node* initrd_load() {
    // create root directory
    initrd_root = kcalloc(1, sizeof(fs_node));
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->readdir = 0;
    initrd_root->finddir = 0;

    initrd_file_count = initrd_count_files();

    // allocate space
    initrd_nodes = kcalloc(initrd_file_count, sizeof(fs_node));
    initrd_files = kmalloc(sizeof(unsigned*) * initrd_file_count);

    tar_header* header = initrd_header;
    for (int i = 0; i < initrd_file_count; i++) {
        fs_node* node = initrd_nodes + i;

        // save a pointer to the actual data
        initrd_files[i] = (unsigned)header + TAR_HEADER_SIZE;

        strncpy(node->name, header->filename, 100);
        node->length = read_octal(header->size, 12);
        node->inode = i;
        node->flags = FS_FILE;
        node->read = &initrd_read;

        header += TAR_HEADER_SIZE + node->length;
    }

    return initrd_root;
}
