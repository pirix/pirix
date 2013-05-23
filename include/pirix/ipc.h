#pragma once

#define VFS_PID 0

#include "semaphore.h"
#include "vector.h"

typedef struct process process;

typedef struct message {
    int tag;
    int data;
} message;

typedef struct channel {
    process* proc;
    message msg_buffer;
    semaphore send_sem;
    semaphore recv_sem;
    vector reply;
    int reply_id;
} channel;

typedef struct connect {
    int cid;
    thread* owner;
    channel* chan;
    message msg_buffer;
} connect;

int ipc_listen();
int ipc_connect(int pid);
int ipc_send(int cid, message* msg);
int ipc_recv(message* msg);
int ipc_reply(int cid, message* msg);
