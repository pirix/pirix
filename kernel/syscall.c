#include <pirix/scheduler.h>
#include <pirix/syscall.h>
#include <pirix/process.h>
#include <pirix/kprint.h>
#include <pirix/ipc.h>
#include <pirix/timer.h>
#include <pirix/sysinfo.h>
#include <pirix/utsname.h>
#include <pirix/string.h>
#include <arch.h>
#include <config.h>

int sys_reboot() {
    return 0;
}

int sys_log(const char* text) {
    thread* t = scheduler_current_thread();
    kprintf("[%i] %s\n", t->process->pid, (char*)text);
}

int sys_print(const char* text, int len) {
    int len_remaining = len;
    while (len_remaining--) {
        kputc(*(text++));
    }
    return len;
}

int sys_sysinfo(struct sysinfo* info) {
    info->uptime = timer_uptime();
    return 0;
}

int sys_uname(struct utsname* uname) {
    memcpy(uname->sysname, "Pirix", 6);
    memcpy(uname->nodename, "", 1);
    memcpy(uname->release, "", 1);
    memcpy(uname->version, VERSION, sizeof(VERSION));
    memcpy(uname->machine, ARCH, sizeof(ARCH));
    return 0;
}

int sys_exit(int status) {
    process_exit(status);
    return 0;
}

int sys_kill(int pid, int sig) {
    process_kill(pid, sig);
    return 0;
}

int sys_wait(int* status) {
    return 0;
}

int sys_fork() {
    return -1;
}

int sys_clone(void* entry, void* stack) {
    thread* t = scheduler_current_thread();

    thread* new = thread_new(entry);
    thread_set_stack(new, stack);
    process_add_thread(t->process, new);
    scheduler_enqueue_thread(new);

    return new->tid;
}

int sys_getpid() {
    thread* t = scheduler_current_thread();
    if (t->process) {
        return t->process->pid;
    }
    else return -1;
}

int sys_gettid() {
    thread* t = scheduler_current_thread();
    return t->tid;
}

int sys_yield() {
    scheduler_switch();
    return 0;
}

int sys_sbrk(int incr) {
    thread* t = scheduler_current_thread();
    if (!t->process) return 0;
    return process_sbrk(t->process, incr);
}

static unsigned* syscall_table[] = {
    [SYS_REBOOT] = (unsigned*)sys_reboot,
    [SYS_LOG] = (unsigned*)sys_log,
    [SYS_PRINT] = (unsigned*)sys_print,
    [SYS_SYSINFO] = (unsigned*)sys_sysinfo,
    [SYS_UNAME] = (unsigned*)sys_uname,
    [SYS_EXIT] = (unsigned*)sys_exit,
    [SYS_KILL] = (unsigned*)sys_kill,
    [SYS_WAIT] = (unsigned*)sys_wait,
    [SYS_FORK] = (unsigned*)sys_fork,
    [SYS_CLONE] = (unsigned*)sys_clone,
    [SYS_GETPID] = (unsigned*)sys_getpid,
    [SYS_GETTID] = (unsigned*)sys_gettid,
    [SYS_YIELD] = (unsigned*)sys_yield,
    [SYS_IPC_LISTEN] = (unsigned*)ipc_listen,
    [SYS_IPC_CONNECT] = (unsigned*)ipc_connect,
    [SYS_IPC_CLOSE] = (unsigned*)ipc_close,
    [SYS_IPC_CONNECT] = (unsigned*)ipc_connect,
    [SYS_IPC_DISCONNECT] = (unsigned*)ipc_disconnect,
    [SYS_IPC_SEND] = (unsigned*)ipc_send,
    [SYS_IPC_RECEIVE] = (unsigned*)ipc_receive,
    [SYS_IPC_REPLY] = (unsigned*)ipc_reply,
    [SYS_SBRK] = (unsigned*)sys_sbrk,

#ifdef __i386
    [SYS_TLS_SET] = (unsigned*)tls_set,
#endif
};

#define MAX_SYSCALL (sizeof(syscall_table)/sizeof(syscall_table[0]))
typedef int (*syscall_handler)(int, ...);

int syscall(int a, int b, int c, int d, int id) {
    if (id > MAX_SYSCALL) return -1;

    unsigned* entry = syscall_table[id];
    if (!entry) return -1;

    syscall_handler handler = (syscall_handler)entry;
    return handler(a, b, c, d);
}
