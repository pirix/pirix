#pragma once

typedef struct message {
    int src;
    int dst;
    int tag;
    int data;
} message;

void ipc_send(message* msg);
void ipc_recv(message* msg);
