#pragma once

typedef struct message {
    int src;
    int tag;
    int data;
} message;

int ipc_send(int dest, message* msg);
int ipc_recv(message* msg);
int ipc_call(int dest, message* msg);
