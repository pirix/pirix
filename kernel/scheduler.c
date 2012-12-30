#include <pirix/kernel.h>
#include <pirix/scheduler.h>
#include <pirix/process.h>
#include <pirix/kheap.h>

// task switch routine in init.S
void task_switch(cpu_state* state);

static thread* current_thread = 0;
static thread* idle_thread = 0;

static thread* queue_head = 0;
static thread* queue_tail = 0;

void scheduler_init() {
    // create idle thread in system mode
    extern void idle();
    idle_thread = thread_new(&idle);
    idle_thread->state->spsr = 0x5F;
    idle_thread->state->usr_r13 = 0x0;
}

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

thread* scheduler_dequeue_thread() {
    if (queue_head) {
        thread* t = queue_head;
        queue_head = queue_head->next;
        return t;
    }
    return 0;
}

thread* scheduler_current_thread() {
    return current_thread;
}

void scheduler_switch() {
    task_switch(current_thread->state);
}

cpu_state* scheduler_schedule(cpu_state* state) {
    if (current_thread) {
        current_thread->state = state;
        if (current_thread->status == RUNNABLE) {
            scheduler_enqueue_thread(current_thread);
        }
    }

    current_thread = scheduler_dequeue_thread();

    if (!current_thread) {
        return idle_thread->state;
    }

    paging_activate_context(current_thread->process->paging_context);

    return current_thread->state;
}
