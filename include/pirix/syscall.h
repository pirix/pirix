#pragma once

#define SYS_REBOOT   0x0000
#define SYS_LOG      0x0001

#define SYS_EXIT     0x1000
#define SYS_KILL     0x1001
#define SYS_WAIT     0x1002
#define SYS_FORK     0x1003
#define SYS_GETPID   0x1004
#define SYS_ISATTY   0x1005
#define SYS_EXECVE   0x1006

#define SYS_LINK     0x2000
#define SYS_UNLINK   0x2001

#define SYS_CLOSE    0x4000
#define SYS_OPEN     0x4001
#define SYS_READ     0x4002
#define SYS_WRITE    0x4003
#define SYS_LSEEK    0x4004
#define SYS_STAT     0x4005
#define SYS_FSTAT    0x4006
