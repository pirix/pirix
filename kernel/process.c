#include <pirix/process.h>
#include <pirix/scheduler.h>
#include <pirix/memory.h>
#include <pirix/kheap.h>
#include <string.h>

static vector processes;

void process_init() {
    vector_init(&processes);
}

process* process_new(paging_context context) {
    process* self = kmalloc(sizeof(process));

    self->context = context;
    self->chan = 0;

    self->heap.start = (unsigned*)0x100000;
    self->heap.size = 0;
    self->heap.used = 0;

    vector_init(&self->threads);
    vector_init(&self->fds);

    int pid = vector_add(&processes, self);
    self->pid = pid;

    return self;
}

process* process_create(void* entry, paging_context context) {
    thread* new_thread = thread_new(entry);
    unsigned stack = memory_alloc();
    paging_map(context, 0x7ffff000, stack, PAGE_PERM_USER);
    thread_set_stack(new_thread, (unsigned*)0x80000000);

    process* self = process_new(context);
    process_add_thread(self, new_thread);
    scheduler_enqueue_thread(new_thread);
    return self;
}

process* process_get(int pid) {
    return vector_get(&processes, pid);
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
        paging_map(self->context, virt, phys, PAGE_PERM_USER);
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
