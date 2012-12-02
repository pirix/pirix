#pragma once

#include "cpu.h"

typedef struct process process;

typedef enum thread_status {
    RUNNABLE,
    WAITING,
    FINISHED,
} thread_status;

typedef struct thread {
    unsigned* irq_stack;
    unsigned* svc_stack;
    thread_status status;
    cpu_state* state;
    process* process;
    struct thread* next;
} thread;

thread* thread_new(void* entry);
void thread_delete(thread* self);
