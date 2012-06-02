#pragma once

// fs_node flags
#define FS_FILE       0x01
#define FS_DIRECTORY  0x02
#define FS_CHARDEVICE 0x04
#define FS_BLOCKEVICE 0x08
#define FS_PIPE       0x10
#define FS_SYMLINK    0x20
#define FS_MOUNTPOINT 0x40

struct fs_node;
struct fs_node_ops;
struct fs_type;

/**
 * fs_node: Represents an filesystem object
 */
typedef struct fs_node {
    unsigned id;
    unsigned device;
    unsigned mask;
    unsigned uid, gid;
    unsigned flags;
    unsigned size;
    struct fs_node_ops* ops;
    struct fs_superblock* sb;
} fs_node;

/**
 * fs_node_ops: Method table of fs_node operations
 */
typedef struct fs_node_ops {
    unsigned (*read)(fs_node*, unsigned, unsigned, char*);
    unsigned (*write)(fs_node*, unsigned, unsigned, const char*);
    void (*open)(fs_node*, char, char);
    void (*close)(fs_node*);
} fs_node_ops;

/**
 * fs_superblock: Contains main data of a mounted filesystem
 */
typedef struct fs_superblock {
    fs_node* root;
    struct fs_type* type;
} fs_superblock;

/**
 * fs_type: Represents a filesystem type
 */
typedef struct fs_type {
    char* name;
    fs_superblock* (*mount)(void*);
} fs_type;

/**
 * file: Represents an opened file
 */
typedef struct file {
    fs_node* node;
    unsigned offset;
} file;

extern fs_node* fs_root;

int fs_init();
void fs_type_register(fs_type* type);
void fs_type_unregister(fs_type* type);
fs_type* fs_type_find(const char* name);
void fs_mount(const char* type, void* flags, const char* path);
fs_node* fs_lookup(const char* path);

/**
 * fs_read: read data from a fs_node
 *
 * @param node The node to read
 * @param off  The offset in bytes from where to start reading
 * @param len  The number of bytes to read
 * @param buff The buffer in which the data is written into
 * @returns The number of bytes actually read
 */
unsigned fs_read(fs_node* node, unsigned off, unsigned len, char* buff);

/**
 * fs_write: write data to a fs_node
 *
 * @param node The node to write to
 * @param off  The offset in bytes from where to start writing
 * @param len  The number of bytes to write
 * @param buff A buffer with the data to write
 * @returns The number of bytes actually written
 */
unsigned fs_write(fs_node* node, unsigned off, unsigned len, const char* buff);

/**
 * fs_open: open a fs_node
 *
 * @param node  The node to open
 * @param read  Read permission (0=no, 1=yes)
 * @param write Write permission (0=no, 1=overwrite, 2=append)
 */
void fs_open(fs_node* node, char read, char write);

/**
 * fs_close: close a fs_node
 *
 * @param node The node to close
 */
void fs_close(fs_node* node);
