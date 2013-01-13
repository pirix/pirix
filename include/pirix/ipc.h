#pragma once

#define ANY_PID -1
#define VFS_PID 0

typedef struct message {
    int src;
    int tag;
    int data;
} message;

int ipc_send(int dst, message* msg);
int ipc_recv(int src, message* msg);
int ipc_call(int dst, message* msg);
