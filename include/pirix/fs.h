#pragma once

#include <sys/dirent.h>

#define FS_FILE       0x01
#define FS_DIRECTORY  0x02
#define FS_CHARDEVICE 0x04
#define FS_BLOCKEVICE 0x08
#define FS_PIPE       0x10
#define FS_SYMLINK    0x20
#define FS_MOUNTPOINT 0x40

struct fs_node;
struct fs_ops;
struct fs_superblock;

typedef struct fs_node {
    unsigned id;
    unsigned device;
    unsigned mask;
    unsigned uid;
    unsigned gid;
    unsigned flags;
    unsigned size;
    struct fs_node_ops* ops;
} fs_node;

typedef struct fs_node_ops {
    unsigned (*read)(fs_node*, unsigned, unsigned, char*);
    unsigned (*write)(fs_node*, unsigned, unsigned, char*);
    void (*open)(fs_node*, char, char);
    void (*close)(fs_node*);
    dirent* (*dir_read)(fs_node*, unsigned);
    fs_node* (*dir_find)(fs_node*, const char*);
} fs_node_ops;

typedef struct fs_superblock {
    fs_node* root;
} fs_superblock;

typedef struct fs_type {
    char* name;
    fs_superblock* (*get_sb)(void*);
} fs_type;

typedef struct file {
    fs_node* node;
    unsigned offset;
} file;

extern fs_node* fs_root;

int fs_init();
void fs_register_type(fs_type* type);
fs_type* fs_find_type(const char* name);
void fs_mount(const char* type, void* flags, const char* path);
fs_node* fs_lookup(const char* path);

unsigned fs_read(fs_node* node, unsigned offset, unsigned size, char* buffer);
unsigned fs_write(fs_node* node, unsigned offset, unsigned size, char* buffer);
void fs_open(fs_node* node, char read, char write);
void fs_close(fs_node* node);
dirent* fs_dir_read(fs_node* node, unsigned index);
fs_node* fs_dir_find(fs_node* node, const char* name);
