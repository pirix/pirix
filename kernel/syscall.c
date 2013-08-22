#include <pirix/scheduler.h>
#include <pirix/syscall.h>
#include <pirix/process.h>
#include <pirix/thread.h>
#include <pirix/kprint.h>
#include <pirix/ipc.h>
#include <pirix/timer.h>
#include <pirix/sysinfo.h>
#include <pirix/utsname.h>
#include <pirix/string.h>
#include <arch/syscall.h>
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

int sys_clone(void* entry, uintptr_t stack) {
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

#define MAX_SYSCALL (sizeof(syscall_table)/sizeof(syscall_table[0]))
typedef int (*syscall_handler)(int, ...);

static uintptr_t syscall_table[] = {
    [SYS_REBOOT] = (uintptr_t)sys_reboot,
    [SYS_LOG] = (uintptr_t)sys_log,
    [SYS_PRINT] = (uintptr_t)sys_print,
    [SYS_SYSINFO] = (uintptr_t)sys_sysinfo,
    [SYS_UNAME] = (uintptr_t)sys_uname,
    [SYS_EXIT] = (uintptr_t)sys_exit,
    [SYS_KILL] = (uintptr_t)sys_kill,
    [SYS_WAIT] = (uintptr_t)sys_wait,
    [SYS_FORK] = (uintptr_t)sys_fork,
    [SYS_CLONE] = (uintptr_t)sys_clone,
    [SYS_GETPID] = (uintptr_t)sys_getpid,
    [SYS_GETTID] = (uintptr_t)sys_gettid,
    [SYS_YIELD] = (uintptr_t)sys_yield,
    [SYS_IPC_LISTEN] = (uintptr_t)ipc_listen,
    [SYS_IPC_CONNECT] = (uintptr_t)ipc_connect,
    [SYS_IPC_CLOSE] = (uintptr_t)ipc_close,
    [SYS_IPC_CONNECT] = (uintptr_t)ipc_connect,
    [SYS_IPC_DISCONNECT] = (uintptr_t)ipc_disconnect,
    [SYS_IPC_SEND] = (uintptr_t)ipc_send,
    [SYS_IPC_RECEIVE] = (uintptr_t)ipc_receive,
    [SYS_IPC_REPLY] = (uintptr_t)ipc_reply,
    [SYS_SBRK] = (uintptr_t)sys_sbrk,

#ifdef __i386
    [SYS_TLS_SET] = (uintptr_t)tls_set,
#endif
};

int syscall(int a, int b, int c, int d, int id) {
    if (id > MAX_SYSCALL) return -1;

    uintptr_t entry = syscall_table[id];
    if (!entry) return -1;

    syscall_handler handler = (syscall_handler)entry;
    return handler(a, b, c, d);
}
