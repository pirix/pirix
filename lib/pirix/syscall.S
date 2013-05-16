#include <config.h>
#include <pirix/syscall.h>

define_syscall sys_exit,    SYS_EXIT, 1
define_syscall sys_log,     SYS_LOG, 1
define_syscall sys_print,   SYS_PRINT, 3
define_syscall sys_sysinfo, SYS_SYSINFO, 1
define_syscall sys_uname,   SYS_UNAME, 1
define_syscall sys_fork,    SYS_FORK, 0
define_syscall sys_getpid,  SYS_GETPID, 0
define_syscall sys_kill,    SYS_KILL, 2
define_syscall sys_sbrk,    SYS_SBRK, 1
define_syscall sys_wait,    SYS_WAIT, 1
define_syscall sys_listen,  SYS_LISTEN, 0
define_syscall sys_connect, SYS_CONNECT, 1
define_syscall sys_send,    SYS_SEND, 2
define_syscall sys_recv,    SYS_RECV, 1
define_syscall sys_reply,   SYS_REPLY, 2
define_syscall sys_yield,   SYS_YIELD, 0
