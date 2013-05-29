#pragma once

// System
#define SYS_REBOOT   0x00
#define SYS_LOG      0x01
#define SYS_PRINT    0x03
#define SYS_SYSINFO  0x04
#define SYS_UNAME    0x05

// Processes
#define SYS_EXIT     0x10
#define SYS_KILL     0x11
#define SYS_WAIT     0x12
#define SYS_FORK     0x13
#define SYS_CLONE    0x14
#define SYS_YIELD    0x15
#define SYS_GETPID   0x16
#define SYS_GETTID   0x17

// IPC
#define SYS_IPC_LISTEN     0x30
#define SYS_IPC_CLOSE      0x31
#define SYS_IPC_CONNECT    0x32
#define SYS_IPC_DISCONNECT 0x33
#define SYS_IPC_SEND       0x34
#define SYS_IPC_RECEIVE    0x35
#define SYS_IPC_REPLY      0x36

// Memory
#define SYS_SBRK     0x60
