#pragma once

#include <kernel/cpu.h>
#include <kernel/paging.h>
#include <kernel/thread.h>

typedef struct heap {
    unsigned* start;
    unsigned size;
    unsigned used;
} heap;

typedef struct process {
    int pid;
    int flags;
    thread* thread;
    paging_context* paging_context;
    heap heap;
    struct process* next;
} process;

process* process_new(paging_context* context);
process* process_create(void* entry, paging_context* context);
process* process_get_current();
void process_add_thread(process* self, thread* thread);
void process_exit(int retval);
void process_kill(int pid, int sig);
void process_wait();

cpu_state* schedule(cpu_state* state);
