#pragma once

#include <arch.h>
#include "thread.h"

void scheduler_init();

/**
 * Switch to the next thread immediately.
 */
void scheduler_switch();

/**
 * Return the current thread.
 */
thread* scheduler_current_thread();

/**
 * Adds a thread to the end of the queue.
 * @param new_thread thread to add
 */
void scheduler_enqueue_thread(thread* new_thread);

/**
 * Removes the thread from the head of the queue and returns it.
 * @return dequeued thread
 * @retval 0 if the queue is empty
 */
thread* scheduler_dequeue_thread();

/**
 * Do the scheduling. Saves the given state for the current thread and
 * returns the state of the next thread.
 *
 * @param regs the registers of the current thread
 * @return registers of the thread to run next
 */
registers* scheduler_schedule(registers* regs);
