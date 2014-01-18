#include <pirix/kernel.h>
#include <pirix/scheduler.h>
#include <pirix/process.h>
#include <pirix/thread.h>
#include <pirix/spinlock.h>
#include <pirix/kheap.h>
#include <arch/cpu.h>
#include <arch.h>

static thread* current_thread = 0;
static thread* idle_thread = 0;

static thread* queue_head = 0;
static thread* queue_tail = 0;
static int queue_lock = 0;

void scheduler_init() {
    // create idle thread in system mode
    extern void idle();
    idle_thread = thread_new(&idle);
    registers_set_kernel_mode(idle_thread->regs);
}

void scheduler_enqueue_thread(thread* new_thread) {
    new_thread->next = 0;

    spin_lock(&queue_lock);

    if (queue_tail) {
        queue_tail->next = new_thread;
    }
    else if (!queue_head) {
        queue_head = new_thread;
    }
    queue_tail = new_thread;

    spin_unlock(&queue_lock);
}

thread* scheduler_dequeue_thread() {
    spin_lock(&queue_lock);
    thread* t = 0;
    if (queue_head) {
        t = queue_head;
        queue_head = queue_head->next;
        if (!queue_head) queue_tail = 0;
    }
    spin_unlock(&queue_lock);
    return t;
}

thread* scheduler_current_thread() {
    return current_thread;
}

process* scheduler_current_process() {
    return current_thread->process;
}

void scheduler_switch() {
    arch_switch_task(current_thread->regs);
}

registers* scheduler_schedule(registers* regs) {
    if (current_thread) {
        current_thread->regs = regs;
        if (current_thread->state == STATE_READY) {
            scheduler_enqueue_thread(current_thread);
        }
    }

    current_thread = scheduler_dequeue_thread();

    if (!current_thread) {
        return idle_thread->regs;
    }

    arch_switch_context(current_thread);

    return current_thread->regs;
}
