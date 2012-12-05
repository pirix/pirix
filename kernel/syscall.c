#include <kernel/scheduler.h>
#include <kernel/syscall.h>
#include <kernel/process.h>
#include <kernel/kprint.h>
#include <kernel/ipc.h>

int syscall(int a, int b, int c, int d, int id) {
    switch (id) {
    case SYS_REBOOT:
        return 0;

    case SYS_LOG:
        kputs((char*)a);
        kputc('\n');
        return 0;

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
        thread* t = scheduler_current();
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

    case SYS_SEND:
        ipc_send(a);
        return 0;

    case SYS_RECV:
        ipc_recv(a);
        return 0;

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
        thread* t = scheduler_current();
        if (!(t && t->process)) return 0;
        process* p = t->process;

        if (a == 0) {
            return p->heap.start + p->heap.size;
        }

        while (a > p->heap.size - p->heap.used) {
            unsigned phys = memory_alloc();
            unsigned virt = p->heap.start + p->heap.size;
            paging_map(p->paging_context, virt, phys, PTE_PERM_USER);
            p->heap.size += 0x1000;
        }

        p->heap.used += a;
        return (int)p->heap.start;
    }

    default:
        return 0;
    }
}
