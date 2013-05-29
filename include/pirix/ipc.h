#pragma once

#include <sys/types.h>
#include "semaphore.h"
#include "vector.h"

typedef struct process process;

typedef struct channel {
    process* proc;
    semaphore send_sem;
    semaphore recv_sem;
    vector reply;
    int recvid;
    char* buffer;
    unsigned int size;
} channel;

typedef struct connect {
    int cid;
    thread* owner;
    channel* chan;
    char* buffer;
    unsigned int size;
    int status;
} connect;

typedef struct msg_buffer {
    char flags;
    size_t size;
    void* data;
} msg_buffer;

int ipc_listen();
int ipc_close(int chanid);
int ipc_connect(int pid, int chainid);
int ipc_disconnect(int cid);
int ipc_send(int cid, msg_buffer* send_buf, msg_buffer* recv_buf);
int ipc_receive(int chainid, msg_buffer* recv_buf);
int ipc_reply(int recvid, int status, msg_buffer* send_buf);

#define MSG_TYPE 0x3
#define MSG_LIN  0x1
#define MSG_VEC  0x2

#define MSG_BUF(_flags, _data, _size)                \
         ((msg_buffer){ .flags = (_flags),           \
                        .size = (_size),             \
                        .data = (void*)(_data) })

#define MSG_BUF_LIN(_data, _size) MSG_BUF(MSG_LIN, _data, _size)
#define MSG_BUF_VEC(_data, _size) MSG_BUF(MSG_VEC, _data, _size)
