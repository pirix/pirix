#pragma once

typedef struct message message;
typedef struct sysinfo sysinfo;

int sys_log(char*);
int sys_exit(int status);
int sys_sysinfo(sysinfo* info);

int sys_getpid();
int sys_kill(int pid, int sig);
int sys_wait(int* status);
int sys_fork();
int sys_yield();
int sys_isatty(int fd);
int sys_sbrk(int incr);

int sys_open(char* f, int flags);
int sys_write(int fd, char* buf, int len);
int sys_read(int fd, char* buf, int len);
int sys_close(int fd);
int sys_link(char* old, char* new);
int sys_unlink(char* name);
int sys_execve(char* name, char** argc, char** env);
int sys_lseek(int fd, int ptr, int dir);
int sys_fstat(int fd, int st);

int sys_send(message* msg);
int sys_recv(message* msg);
