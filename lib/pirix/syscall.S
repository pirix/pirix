#include <config.h>
#include <pirix/syscall.h>

define_syscall sys_exit,     SYS_EXIT, 1
define_syscall sys_log,      SYS_LOG, 1
define_syscall sys_print,    SYS_PRINT, 3
define_syscall sys_sysinfo,  SYS_SYSINFO, 1
define_syscall sys_uname,    SYS_UNAME, 1
define_syscall sys_fork,     SYS_FORK, 0
define_syscall sys_clone,    SYS_CLONE, 3
define_syscall sys_getpid,   SYS_GETPID, 0
define_syscall sys_gettid,   SYS_GETTID, 0
define_syscall sys_kill,     SYS_KILL, 2
define_syscall sys_sbrk,     SYS_SBRK, 1
define_syscall sys_wait,     SYS_WAIT, 1
define_syscall sys_yield,    SYS_YIELD, 0

define_syscall ipc_listen,     SYS_IPC_LISTEN, 0
define_syscall ipc_close,      SYS_IPC_CLOSE, 1
define_syscall ipc_connect,    SYS_IPC_CONNECT, 2
define_syscall ipc_disconnect, SYS_IPC_DISCONNECT, 1
define_syscall ipc_send,       SYS_IPC_SEND, 3
define_syscall ipc_receive,    SYS_IPC_RECEIVE, 2
define_syscall ipc_reply,      SYS_IPC_REPLY, 3
