#pragma once

typedef struct process process;

typedef enum thread_status {
    RUNNABLE,
    RUNNING,
    WAITING,
} thread_status;

typedef struct thread {
    unsigned* irq_stack;
    unsigned* svc_stack;
    thread_status status;
    cpu_state* state;
    process* process;
} thread;

thread* thread_new(void* entry);
void thread_delete(thread* self);
