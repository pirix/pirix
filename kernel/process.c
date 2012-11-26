#include <kernel/process.h>
#include <kernel/kheap.h>

static process* first_process = 0;
static process* current_process = 0;

int pid_counter = 0;

process* process_new(paging_context* context) {
    process* new_process = kmalloc(sizeof(process));
    new_process->pid = pid_counter++;
    new_process->paging_context = context;

    new_process->heap.start = 0x100000;
    new_process->heap.size = 0;
    new_process->heap.used = 0;

    // prepend to process list
    new_process->next = first_process;
    first_process = new_process;

    return new_process;
}

process* process_create(void* entry, paging_context* context) {
    process* new_process = process_new(context);
    process_add_thread(new_process, thread_new(entry));
    return new_process;
}

void process_add_thread(process* self, thread* thread) {
    self->thread = thread;
    thread->process = self;
}

process* process_get(int pid) {
    process* p = first_process;
    while (p != 0) {
        if (p->pid == pid) return p;
        p = p->next;
    }
    return 0;
}

process* process_get_current() {
    return current_process;
}

void process_kill(int pid, int sig) {
    return;
}

void process_exit(int retval) {
    process* p = first_process;
    while (p->next != current_process) {
        p = p->next;
    }
    p->next = current_process->next;
}

void process_wait() {
    current_process->thread->status = WAITING;
}

cpu_state* schedule(cpu_state* state) {
    if (current_process) {
        current_process->thread->state = state;
    }

    if (current_process && current_process->next) {
        current_process = current_process->next;
    }
    else if (first_process) {
        current_process = first_process;
    }
    else {
        return state;
    }

    paging_activate_context(current_process->paging_context);

    return current_process->thread->state;
}
