#include <kernel/syscall.h>

.global sys_exit
sys_exit:
    swi #SYS_EXIT
    bx lr

.global sys_log
sys_log:
    swi #SYS_LOG
    bx lr

.global sys_sysinfo
sys_sysinfo:
    swi #SYS_SYSINFO
    bx lr

.global sys_execve
sys_execve:
    swi #SYS_EXECVE
    bx lr

.global sys_close
sys_close:
    swi #SYS_CLOSE
    bx lr

.global sys_fork
sys_fork:
    swi #SYS_FORK
    bx lr

.global sys_fstat
sys_fstat:
    swi #SYS_FSTAT
    bx lr

.global sys_getpid
sys_getpid:
    swi #SYS_GETPID
    bx lr

.global sys_isatty
sys_isatty:
    swi #SYS_ISATTY
    bx lr

.global sys_kill
sys_kill:
    swi #SYS_KILL
    bx lr

.global sys_link
sys_link:
    swi #SYS_LINK
    bx lr

.global sys_lseek
sys_lseek:
    swi #SYS_LSEEK
    bx lr

.global sys_open
sys_open:
    swi #SYS_OPEN
    bx lr

.global sys_read
sys_read:
    swi #SYS_READ
    bx lr

.global sys_write
sys_write:
    swi #SYS_WRITE
    bx lr

.global sys_unlink
sys_unlink:
    swi #SYS_UNLINK
    bx lr

.global sys_wait
sys_wait:
    swi #SYS_WAIT
    bx lr

.global sys_send
sys_send:
    swi #SYS_SEND
    bx lr

.global sys_recv
sys_recv:
    swi #SYS_RECV
    bx lr

.global sys_yield
sys_yield:
    swi #SYS_YIELD
    bx lr
