#include <pirix/scheduler.h>
#include <pirix/syscall.h>
#include <pirix/process.h>
#include <pirix/kprint.h>
#include <pirix/ipc.h>
#include <pirix/timer.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <string.h>
#include <config.h>

int syscall(int a, int b, int c, int d, int id) {
    switch (id) {
    case SYS_REBOOT:
        return 0;

    case SYS_LOG: {
        thread* t = scheduler_current_thread();
        kprintf("[%i] %s\n", t->process->pid, (char*)a);
        return 0;
    }

    case SYS_PRINT: {
        char* buf = (char*)a;
        int len = b;
        for (int i = 0; i < len; i++) {
            kputc(*(buf++));
        }
        return len;
    }

    case SYS_SYSINFO: {
        sysinfo* info = (sysinfo*)a;
        info->uptime = timer_uptime();
        return 0;
    }

    case SYS_UNAME: {
        struct utsname* un = (struct utsname*)a;
        memcpy(un->sysname, "Pirix", 6);
        memcpy(un->nodename, "", 1);
        memcpy(un->release, "", 1);
        memcpy(un->version, VERSION, sizeof(VERSION));
        memcpy(un->machine, ARCH, sizeof(ARCH));
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

    case SYS_CLONE: {
        thread* t = scheduler_current_thread();

        void* entry = (void*)a;
        void* stack = (void*)b;

        thread* new = thread_new(entry);
        thread_set_stack(new, stack);
        process_add_thread(t->process, new);
        scheduler_enqueue_thread(new);

        return new->tid;
    }

    case SYS_GETPID: {
        thread* t = scheduler_current_thread();
        if (t->process) {
            return t->process->pid;
        }
        else return -1;
    }

    case SYS_GETTID: {
        thread* t = scheduler_current_thread();
        return t->tid;
    }

    case SYS_YIELD:
        scheduler_switch();
        return 0;

    case SYS_IPC_LISTEN:
        return ipc_listen();

    case SYS_IPC_CLOSE:
        return ipc_close((int)a);

    case SYS_IPC_CONNECT:
        return ipc_connect((int)a, (int)b);

    case SYS_IPC_DISCONNECT:
        return ipc_disconnect((int)a);

    case SYS_IPC_SEND:
        return ipc_send((int)a, (msg_buffer*)b, (msg_buffer*)c);

    case SYS_IPC_RECEIVE:
        return ipc_receive((int)a, (msg_buffer*)b);

    case SYS_IPC_REPLY:
        return ipc_reply((int)a, (int)b, (msg_buffer*)c);

    case SYS_SBRK: {
        thread* t = scheduler_current_thread();
        if (!(t && t->process)) return 0;
        int ret = process_sbrk(t->process, a);
        return ret;
    }

    default:
        return 0;
    }
}
