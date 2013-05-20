#pragma once

typedef struct thread thread;

typedef struct semaphore {
    int count;
    int lock;
    thread* queue;
} semaphore;

void semaphore_init(semaphore* sem, int count);
void semaphore_wait(semaphore* sem);
void semaphore_signal(semaphore* sem);
