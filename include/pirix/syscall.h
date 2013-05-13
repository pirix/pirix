#pragma once

// System
#define SYS_REBOOT   0x00
#define SYS_LOG      0x01
#define SYS_SYSINFO  0x02
#define SYS_PRINT    0x03

// Processes
#define SYS_EXIT     0x10
#define SYS_KILL     0x11
#define SYS_WAIT     0x12
#define SYS_FORK     0x13
#define SYS_GETPID   0x14
#define SYS_YIELD    0x15

// IPC
#define SYS_LISTEN   0x30
#define SYS_CONNECT  0x31
#define SYS_SEND     0x32
#define SYS_RECV     0x33
#define SYS_REPLY    0x34

// Memory
#define SYS_SBRK     0x60
