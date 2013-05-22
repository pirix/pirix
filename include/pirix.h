#pragma once

typedef struct message message;
typedef struct sysinfo sysinfo;
struct utsname;

int sys_exit(int status);
int sys_log(char* buf);
int sys_print(char* buf, int len);
int sys_sysinfo(sysinfo* info);
int sys_uname(struct utsname*);

int sys_kill(int pid, int sig);
int sys_wait(int* status);
int sys_fork();
int sys_clone(void* (*func)(void*), void* stack, int flags);
int sys_yield();
int sys_getpid();
int sys_gettid();
int sys_sbrk(int incr);

int sys_listen();
int sys_connect(int pid);
int sys_send(int cid, message* msg);
int sys_recv(message* msg);
int sys_reply(int cid, message* msg);
