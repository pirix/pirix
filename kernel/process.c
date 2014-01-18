#include <pirix/process.h>
#include <pirix/thread.h>
#include <pirix/scheduler.h>
#include <pirix/frame.h>
#include <pirix/kheap.h>
#include <pirix/memarea.h>
#include <pirix/string.h>

static vector processes;

void process_init() {
    vector_init(&processes, 16);
}

process* process_new(addrspace* as) {
    process* self = kmalloc(sizeof(process));

    self->chan = 0;
    self->as = as;

    vector_init(&self->threads, 4);
    vector_init(&self->fds, 8);
    vector_init(&self->chans, 0);

    int pid = vector_add(&processes, self);
    self->pid = pid;

    return self;
}

process* process_create(void* entry, addrspace* as) {
    thread* new_thread = thread_new(entry);

    memarea* stack = memarea_new(0, MEM_READ | MEM_WRITE | MEM_GROWDOWN);
    stack->start = 0x7ffff000;
    stack->end = 0x80000000;

    addrspace_add_area(as, stack);

    thread_set_stack(new_thread, 0x80000000-0xc);

    process* self = process_new(as);
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

uintptr_t process_sbrk(process* self, int incr) {
    return addrspace_sbrk(self->as, incr);
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
