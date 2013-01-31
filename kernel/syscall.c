#include <pirix/scheduler.h>
#include <pirix/syscall.h>
#include <pirix/process.h>
#include <pirix/kprint.h>
#include <pirix/ipc.h>
#include <pirix/timer.h>
#include <sys/sysinfo.h>

int syscall(int a, int b, int c, int d, int id) {
    switch (id) {
    case SYS_REBOOT:
        return 0;

    case SYS_LOG: {
        thread* t = scheduler_current_thread();
        kprintf("[%i] %s\n", t->process->pid, (char*)a);
        return 0;
    }

    case SYS_EXIT:
        process_exit(a);
        return 0;

    case SYS_KILL:
        process_kill(a, b);
        return -1;

    case SYS_WAIT:
        return 0;

    case SYS_FORK:
        return -1;

    case SYS_GETPID: {
        thread* t = scheduler_current_thread();
        if (t && t->process) {
            return t->process->pid;
        }
        else return -1;
    }

    case SYS_YIELD:
        scheduler_switch();
        return 0;

    case SYS_ISATTY:
        return 1;

    case SYS_EXECVE:
        return -1;

    case SYS_LINK:
        return -1;

    case SYS_UNLINK:
        return -1;

    case SYS_LISTEN:
        return ipc_listen();

    case SYS_CONNECT:
        return ipc_connect((int)a);

    case SYS_SEND:
        return ipc_send((int)a, (message*)b);

    case SYS_RECV:
        return ipc_recv((message*)a);

    case SYS_REPLY:
        return ipc_reply((int)a, (message*)b);

    case SYS_CLOSE:
        return -1;

    case SYS_OPEN:
        return -1;

    case SYS_READ:
        return 0;

    case SYS_WRITE: {
        int len = c;
        char* data = (char*)b;

        for (int i = 0; i < c; i++) {
            kputc(*data++);
        }
        return len;
    }

    case SYS_LSEEK:
        return 0;

    case SYS_STAT:
        return 0;

    case SYS_FSTAT:
        return 0;

    case SYS_SBRK: {
        thread* t = scheduler_current_thread();
        if (!(t && t->process)) return 0;
        return (int)process_sbrk(t->process, (unsigned)a);
    }

    case SYS_SYSINFO: {
        sysinfo* info = (sysinfo*)a;
        info->uptime = timer_uptime();
        return 0;
    }

    default:
        return 0;
    }
}
