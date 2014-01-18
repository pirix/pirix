#pragma once

typedef struct thread thread;
typedef struct process process;
typedef struct registers registers;

/**
 * @defgroup Scheduler Scheduling
 * Scheduling functions
 */

/**
 * Initialize scheduling.
 *
 * @ingroup Scheduler
 */
void scheduler_init();

/**
 * Switch to the next thread immediately.
 *
 * @ingroup Scheduler
 */
void scheduler_switch();

/**
 * Returns the current thread.
 *
 * @ingroup Scheduler
 */
thread* scheduler_current_thread();

/**
 * Returns the current process.
 *
 * @ingroup Scheduler
 */
process* scheduler_current_process();

/**
 * Adds a thread to the end of the queue.
 * @param new_thread thread to add
 */
void scheduler_enqueue_thread(thread* new_thread);

/**
 * Removes the thread from the head of the queue and returns it.
 *
 * @return dequeued thread
 * @retval 0 if the queue is empty
 * @ingroup Scheduler
 */
thread* scheduler_dequeue_thread();

/**
 * Do the scheduling. Saves the given state for the current thread and
 * returns the state of the next thread.
 *
 * @param regs the registers of the current thread
 * @return registers of the thread to run next
 * @ingroup Scheduler
 */
registers* scheduler_schedule(registers* regs);
