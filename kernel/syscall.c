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

static syscall_handler syscall_table[] = {
    [SYS_REBOOT] = (syscall_handler)sys_reboot,
    [SYS_LOG] = (syscall_handler)sys_log,
    [SYS_PRINT] = (syscall_handler)sys_print,
    [SYS_SYSINFO] = (syscall_handler)sys_sysinfo,
    [SYS_UNAME] = (syscall_handler)sys_uname,
    [SYS_EXIT] = (syscall_handler)sys_exit,
    [SYS_KILL] = (syscall_handler)sys_kill,
    [SYS_WAIT] = (syscall_handler)sys_wait,
    [SYS_FORK] = (syscall_handler)sys_fork,
    [SYS_CLONE] = (syscall_handler)sys_clone,
    [SYS_GETPID] = (syscall_handler)sys_getpid,
    [SYS_GETTID] = (syscall_handler)sys_gettid,
    [SYS_YIELD] = (syscall_handler)sys_yield,
    [SYS_IPC_LISTEN] = (syscall_handler)ipc_listen,
    [SYS_IPC_CONNECT] = (syscall_handler)ipc_connect,
    [SYS_IPC_CLOSE] = (syscall_handler)ipc_close,
    [SYS_IPC_CONNECT] = (syscall_handler)ipc_connect,
    [SYS_IPC_DISCONNECT] = (syscall_handler)ipc_disconnect,
    [SYS_IPC_SEND] = (syscall_handler)ipc_send,
    [SYS_IPC_RECEIVE] = (syscall_handler)ipc_receive,
    [SYS_IPC_REPLY] = (syscall_handler)ipc_reply,
    [SYS_SBRK] = (syscall_handler)sys_sbrk,

#ifdef __i386
    [SYS_TLS_SET] = (syscall_handler)tls_set,
#endif
};

int syscall(int a, int b, int c, int d, int id) {
    int max_syscall = sizeof(syscall_table)/sizeof(syscall_table[0]);
    if (id > max_syscall) return -1;

    syscall_handler handler = syscall_table[id];
    if (!handler) return -1;

    return handler(a, b, c, d);
}
