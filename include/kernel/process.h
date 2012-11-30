#pragma once

#include "paging.h"
#include "thread.h"

typedef struct heap {
    unsigned* start;
    unsigned size;
    unsigned used;
} heap;

typedef struct process {
    int pid;
    int flags;
    heap heap;
    thread* thread;
    paging_context* paging_context;
    struct process* next;
} process;

process* process_new(paging_context* context);
process* process_create(void* entry, paging_context* context);
void process_add_thread(process* self, thread* thread);
void process_exit(int retval);
void process_kill(int pid, int sig);
