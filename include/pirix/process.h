#pragma once

#include <pirix/types.h>
#include <pirix/addrspace.h>
#include <pirix/vector.h>

typedef struct thread thread;
typedef struct channel channel;

/**
 * The process control block.
 * Holds all information about a process.
 */
typedef struct process {
    int pid;
    int flags;
    vector threads;
    vector fds;
    vector chans;
    channel* chan;
    addrspace* as;
} process;

/**
 * Initialize processes.
 */
void process_init();

/**
 * Create an empty process.
 *
 * @param context The paging context for the process.
 * @return The new process.
 * @memberof process
 */
process* process_new(addrspace* as);

/**
 * Create a new process object with a thread.
 *
 * @param entry The entry point for the thread.
 * @param context The paging context for the process.
 * @return The new process.
 * @memberof process
 */
process* process_create(void* entry, addrspace* as);

/**
 * Get a process by its pid.
 *
 * @param pid The process id to search for.
 * @memberof process
 */
process* process_get(int pid);

/**
 * Add a thread to a process.
 *
 * @param thread The thread to add.
 * @memberof process
 */
void process_add_thread(process* self, thread* thread);

/**
 * Remove a thread from a process.
 *
 * @param thread The thread to remove.
 * @memberof process
 */
void process_remove_thread(process* self, thread* thread);

/**
 * Increase the heap size of a process.
 *
 * @param incr The amount of bytes to allocate.
 * @memberof process
 */
uintptr_t process_sbrk(process* self, int incr);

/**
 * Exit the current process.
 *
 * @param retval The exit code.
 * @memberof process
 */
void process_exit(int retval);

/**
 * Send a signal to a process.
 *
 * @param pid The process id of the process to signal.
 * @param sig The signal id.
 * @memberof process
 */
void process_kill(int pid, int sig);
