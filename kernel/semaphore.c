#include <pirix/semaphore.h>
#include <pirix/scheduler.h>
#include <pirix/spinlock.h>
#include <pirix/thread.h>
#include <arch.h>

void semaphore_init(semaphore* sem, int count) {
    sem->count = count;
    sem->queue = 0;
    sem->lock = 0;
}

void semaphore_wait(semaphore* sem) {
    spin_lock(&sem->lock);

    if (sem->count > 0) {
        sem->count--;
        spin_unlock(&sem->lock);
        return;
    }

    thread* t = scheduler_current_thread();

    // enqueue thread
    t->next = 0;
    if (!sem->queue) sem->queue = t;
    else {
        thread* curr = sem->queue;
        while (curr->next) curr = curr->next;
        curr->next = t;
    }

    spin_unlock(&sem->lock);

    thread_block(t, STATE_SEMA);
    scheduler_switch();
}

void semaphore_signal(semaphore* sem) {
    spin_lock(&sem->lock);

    if (!sem->queue) {
        sem->count++;
    }
    else {
        thread* t = sem->queue;
        sem->queue = t->next;
        thread_unblock(t);
    }

    spin_unlock(&sem->lock);
}
