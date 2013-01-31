#pragma once

#define ANY_PID -1
#define VFS_PID 0

typedef struct process process;

typedef struct message {
    int tag;
    int data;
} message;

typedef struct channel {
    process* proc;
    message msg;
} channel;

typedef struct connect {
    channel* chan;
    int cid;
} connect;

int ipc_listen();
int ipc_connect(int pid);
int ipc_send(int cid, message* msg);
int ipc_recv(message* msg);
int ipc_reply(int cid, message* msg);
