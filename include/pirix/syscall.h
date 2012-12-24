#pragma once

// System
#define SYS_REBOOT   0x00
#define SYS_LOG      0x01
#define SYS_SYSINFO  0x02

// Processes
#define SYS_EXIT     0x10
#define SYS_KILL     0x11
#define SYS_WAIT     0x12
#define SYS_FORK     0x13
#define SYS_GETPID   0x14
#define SYS_ISATTY   0x15
#define SYS_EXECVE   0x16
#define SYS_YIELD    0x17

// IPC
#define SYS_SEND     0x30
#define SYS_RECV     0x31

// Filesystem
#define SYS_CLOSE    0x40
#define SYS_OPEN     0x41
#define SYS_READ     0x42
#define SYS_WRITE    0x43
#define SYS_LSEEK    0x44
#define SYS_STAT     0x45
#define SYS_FSTAT    0x46
#define SYS_LINK     0x47
#define SYS_UNLINK   0x48

// Memory
#define SYS_SBRK     0x60
