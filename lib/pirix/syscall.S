#include <pirix/syscall.h>

.macro define_syscall name, id
.global \name
\name:
    swi \id
    bx lr
.endm

define_syscall sys_exit,    #SYS_EXIT
define_syscall sys_log,     #SYS_LOG
define_syscall sys_sysinfo, #SYS_SYSINFO
define_syscall sys_execve,  #SYS_EXECVE
define_syscall sys_fork,    #SYS_FORK
define_syscall sys_fstat,   #SYS_FSTAT
define_syscall sys_getpid,  #SYS_GETPID
define_syscall sys_isatty,  #SYS_ISATTY
define_syscall sys_kill,    #SYS_KILL
define_syscall sys_sbrk,    #SYS_SBRK
define_syscall sys_link,    #SYS_LINK
define_syscall sys_lseek,   #SYS_LSEEK
define_syscall sys_open,    #SYS_OPEN
define_syscall sys_close,   #SYS_CLOSE
define_syscall sys_read,    #SYS_READ
define_syscall sys_write,   #SYS_WRITE
define_syscall sys_unlink,  #SYS_UNLINK
define_syscall sys_wait,    #SYS_WAIT
define_syscall sys_send,    #SYS_SEND
define_syscall sys_recv,    #SYS_RECV
define_syscall sys_call,    #SYS_CALL
define_syscall sys_yield,   #SYS_YIELD
