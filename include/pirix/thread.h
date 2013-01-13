#pragma once

#include "cpu.h"

typedef struct process process;
typedef struct message message;

typedef enum thread_state {
    STATE_READY,
    STATE_WAIT,
    STATE_RECV,
    STATE_DEAD,
} thread_state;

typedef struct thread {
    int tid;
    unsigned* svc_stack;
    unsigned* thr_stack;
    thread_state state;
    cpu_state* registers;
    process* process;
    message* msg;
    struct thread* next;
} thread;

/**
 * Create a new thread.
 * @param entry entry point
 * @memberof thread
 */
thread* thread_new(void* entry);


/**
 * Set the stack of a thread.
 * @memberof thread
 */
void thread_set_stack(thread* self, unsigned* addr);

/**
 * Block a thread by changing its state. The thread has to be removed
 * from the scheduler queue before.
 * @param state the blocking state
 * @memberof thread
 */
void thread_block(thread* self, thread_state state);


/**
 * Change the state of a thread to READY and add it to the scheduler
 * queue.
 * @memberof thread
 */
void thread_unblock(thread* self);

/**
 * Delete a thread.
 * @memberof thread
 */
void thread_delete(thread* self);
