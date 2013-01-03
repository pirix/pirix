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

    return new_process;
}

process* process_create(void* entry, paging_context* context) {
    process* new_process = process_new(context);
    thread* new_thread = thread_new(entry);
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
    for (int i = 0; i < MAX_THREADS; i++) {
        if (!self->threads[i]) {
            self->threads[i] = thread;
            thread->process = self;
            thread_init_stack(thread, i);
            return;
        }
    }
}

void process_remove_thread(process* self, thread* thread) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (self->threads[i] == thread) {
            self->threads[i] = 0;
            thread->process = 0;
            return;
        }
    }
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
    thread->status = FINISHED;
    scheduler_switch();
}
