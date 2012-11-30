#include <kernel/scheduler.h>
#include <kernel/process.h>
#include <kernel/kheap.h>

static thread* current_thread = 0;

static thread* queue_head = 0;
static thread* queue_tail = 0;

void scheduler_enqueue_thread(thread* new_thread) {
    new_thread->next = 0;

    if (queue_tail) {
        queue_tail->next = new_thread;
    }

    queue_tail = new_thread;

    if (!queue_head) {
        queue_head = new_thread;
    }
}

void scheduler_switch() {
    // @todo: save state of current_thread
    current_thread = scheduler_dequeue_thread();
    task_switch(current_thread->state);
}

thread* scheduler_current() {
    return current_thread;
}

thread* scheduler_dequeue_thread() {
    if (queue_head) {
        thread* t = queue_head;
        queue_head = queue_head->next;
        return t;
    }
    return 0;
}

cpu_state* scheduler_schedule(cpu_state* state) {
    if (current_thread) {
        scheduler_enqueue_thread(current_thread);
        current_thread->state = state;
    }

    current_thread = scheduler_dequeue_thread();

    if (!current_thread) return state;

    paging_activate_context(current_thread->process->paging_context);

    return current_thread->state;
}
