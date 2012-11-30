#include "kernel/ipc.h"

void sys_log(const char*);
void sys_exit(int status);
void sys_kill(int pid, int sig);
void sys_wait();
int sys_getpid();
void sys_send(message* msg);
message* sys_recv();
