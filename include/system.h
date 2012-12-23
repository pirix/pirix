#include "kernel/ipc.h"

typedef struct sysinfo sysinfo;

void sys_log(const char*);
void sys_exit(int status);
int sys_sysinfo(sysinfo* info);
void sys_kill(int pid, int sig);
void sys_wait();
void sys_yield();
int sys_getpid();
void sys_send(message* msg);
message* sys_recv();
