#include <pirix/fs.h>
#include <pirix/kheap.h>

#define FILES_OPEN_MAX 100

static file* files_open[FILES_OPEN_MAX];

unsigned file_open(const char* path) {
    unsigned fd = 1;
    while (files_open[fd] && fd < FILES_OPEN_MAX) {
        fd++;
    }
    if (fd == FILES_OPEN_MAX) return 0;

    file* f = kcalloc(1, sizeof(file));
    f->node = fs_lookup(path);

    if (f->node) {
        files_open[fd] = f;
        return fd;
    }
    else {
        kfree(f, sizeof(file));
        return 0;
    }
}

unsigned file_read(unsigned fd, char* buff, size_t size) {
    file* f = files_open[fd];
    int len = fs_read(f->node, f->offset, size, buff);
    f->offset += len;
    return len;
}

unsigned file_write(unsigned fd, char* buff, size_t size) {
    file* f = files_open[fd];
    int len = fs_write(f->node, f->offset, size, buff);
    f->offset += len;
    return len;
}

void file_close(unsigned fd) {
    file* f = files_open[fd];
    fs_close(f->node);
    files_open[fd] = 0;
    kfree(f, sizeof(file));
}
