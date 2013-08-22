#pragma once

typedef struct thread thread;

typedef struct semaphore {
    int count;
    int lock;
    thread* queue;
} semaphore;

/**
 * Initialize a semaphore.
 * @param count The initial count.
 * @memberof semaphore
 */
void semaphore_init(semaphore* sem, int count);

/**
 * Try to decrement the semaphore counter.
 * Blocks if the counter is zero.
 * @memberof semaphore
 */
void semaphore_wait(semaphore* sem);

/**
 * Increment the semaphore counter.
 * Unblocks waiting processes.
 * @memberof semaphore
 */
void semaphore_signal(semaphore* sem);
