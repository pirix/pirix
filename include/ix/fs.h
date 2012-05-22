#ifndef IX_FS_H
#define IX_FS_H

#define FS_FILE       0x01
#define FS_DIRECTORY  0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKEVICE 0x04
#define FS_PIPE       0x05
#define FS_SYMLINK    0x06
#define FS_MOUNTPOINT 0x08

struct fs_node;
struct dirent;

typedef unsigned (read_handler)(struct fs_node*, unsigned, unsigned, char*);
typedef unsigned (write_handler)(struct fs_node*, unsigned, unsigned, char*);
typedef void (open_handler)(struct fs_node*);
typedef void (close_handler)(struct fs_node*);
typedef struct dirent* (readdir_handler)(struct fs_node*, unsigned);
typedef struct fs_node* (finddir_handler)(struct fs_node*, char *name);

typedef struct fs_node {
    char name[128];
    unsigned mask;
    unsigned uid;
    unsigned gid;
    unsigned flags;
    unsigned length;
    unsigned impl;

    read_handler* read;
    write_handler* write;
    open_handler* open;
    close_handler* close;
    readdir_handler* readdir;
    finddir_handler* finddir;

    struct fs_node* ptr;
} fs_node;

typedef struct dirent {
    char name[128];
    unsigned ino;
} dirent;

extern fs_node* fs_root;

unsigned fs_read(fs_node* node, unsigned offset, unsigned size, char* buffer);
unsigned fs_write(fs_node* node, unsigned offset, unsigned size, char* buffer);
void fs_open(fs_node* node, char read, char write);
void fs_close(fs_node* node);
dirent* fs_readdir(fs_node* node, unsigned index);
fs_node* fs_finddir(fs_node* node, char* name);

#endif
