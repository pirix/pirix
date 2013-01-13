#include <pirix/process.h>
#include <pirix/scheduler.h>
#include <pirix/memory.h>
#include <pirix/kheap.h>
#include <string.h>

static process* first_process = 0;
int pid_counter = 1;

process* process_new(paging_context* context) {
    process* new_process = kmalloc(sizeof(process));
    new_process->pid = pid_counter++;
    new_process->context = context;

    new_process->heap.start = (unsigned*)0x100000;
    new_process->heap.size = 0;
    new_process->heap.used = 0;

    // prepend to process list
    new_process->next = first_process;
    first_process = new_process;

    vector_init(&new_process->threads);

    return new_process;
}

process* process_create(void* entry, paging_context* context) {
    thread* new_thread = thread_new(entry);
    unsigned stack = memory_alloc();
    paging_map(context, 0x7ffff000, stack, PTE_PERM_USER);
    thread_set_stack(new_thread, (unsigned*)0x80000000);

    process* new_process = process_new(context);
    process_add_thread(new_process, new_thread);
    scheduler_enqueue_thread(new_thread);
    return new_process;
}

process* process_get(int pid) {
    process* p = first_process;
    while (p != 0) {
        if (p->pid == pid) return p;
        p = p->next;
    }
    return 0;
}

void process_add_thread(process* self, thread* thread) {
    int tid = vector_add(&self->threads, thread);
    thread->tid = tid;
    thread->process = self;
}

void process_remove_thread(process* self, thread* thread) {
    vector_set(&self->threads, thread->tid, 0);
}

unsigned* process_sbrk(process* self, unsigned incr) {
    if (incr == 0) {
        return self->heap.start + self->heap.size;
    }

    while (incr > self->heap.size - self->heap.used) {
        unsigned phys = memory_alloc();
        unsigned virt = (unsigned)self->heap.start + self->heap.size;
        paging_map(self->context, virt, phys, PTE_PERM_USER);
        self->heap.size += 0x1000;
    }

    self->heap.used += incr;
    return self->heap.start;
}

void process_kill(int pid, int sig) {
    return;
}

void process_exit(int retval) {
    thread* thread = scheduler_current_thread();
    process* self = thread->process;
    thread_block(thread, STATE_DEAD);
    scheduler_switch();
}
