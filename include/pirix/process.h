#pragma once

#include "vector.h"
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
    vector threads;
    paging_context* context;
    struct process* next;
} process;

/**
 * Create an empty process.
 * @param context paging context for the process
 * @return new process
 * @memberof process
 */
process* process_new(paging_context* context);

/**
 * Create a new process object with a thread.
 * @param entry entry point for the thread
 * @param context paging context for the process
 * @return new process
 * @memberof process
 */
process* process_create(void* entry, paging_context* context);

/**
 * Get a process.
 * @param pid process id
 * @memberof process
 */
process* process_get(int pid);

/**
 * Add a thread to a process.
 * @memberof process
 */
void process_add_thread(process* self, thread* thread);

/**
 * Remove a thread from a process.
 * @memberof process
 */
void process_remove_thread(process* self, thread* thread);

/**
 * Increase the heap size of a process.
 * @memberof process
 */
unsigned* process_sbrk(process* self, unsigned incr);

/**
 * Exit the current process.
 * @param retval exit code
 * @memberof process
 */
void process_exit(int retval);

/**
 * Send a signal to a process.
 * @param pid process id
 * @param sig signal id
 * @memberof process
 */
void process_kill(int pid, int sig);
