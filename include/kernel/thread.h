#pragma once

#include "cpu.h"

typedef struct process process;

typedef enum thread_status {
    RUNNABLE,
    WAITING,
    RECEIVING,
    FINISHED,
} thread_status;

typedef struct thread {
    unsigned* svc_stack;
    thread_status status;
    cpu_state* state;
    process* process;
    struct thread* next;
} thread;

/**
 * Create a new thread.
 * @param entry entry point
 * @memberof thread
 */
thread* thread_new(void* entry);

/**
 * Delete a thread.
 * @memberof thread
 */
void thread_delete(thread* self);
