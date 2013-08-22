#pragma once

#include <pirix/types.h>

typedef struct registers registers;
typedef struct process process;
typedef struct message message;

typedef enum thread_state {
    STATE_READY,
    STATE_WAIT,
    STATE_REPLY,
    STATE_SEMA,
    STATE_DEAD,
} thread_state;

typedef struct thread {
    int tid;
    uintptr_t svc_stack;
    uintptr_t thr_stack;
    thread_state state;
    registers* regs;
    process* process;
    struct thread* next;
#ifdef __i386
    uint32_t tls[2];
#endif
} thread;

/**
 * Create a new thread.
 * @param entry The program entry point.
 * @memberof thread
 */
thread* thread_new(void* entry);

/**
 * Set the stack of a thread.
 * @param addr The new stack address.
 * @memberof thread
 */
void thread_set_stack(thread* self, uintptr_t addr);

/**
 * Block a thread by changing its state. The thread has to be removed
 * from the scheduler queue before.
 * @param state The blocking state.
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
