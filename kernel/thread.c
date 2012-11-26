#include <kernel/cpu.h>
#include <kernel/thread.h>
#include <kernel/kheap.h>
#include <string.h>

#define STACK_SIZE 1024

thread* thread_new(void* entry) {
    unsigned* irq_stack = kmalloc(STACK_SIZE*sizeof(unsigned));
    unsigned* svc_stack = kmalloc(STACK_SIZE*sizeof(unsigned));

    cpu_state* new_state = (cpu_state*)(irq_stack + STACK_SIZE) - 1;

    *new_state = (cpu_state) {
        .usr_r13 = 0x80000000,
        .svc_r13 = (unsigned)(svc_stack + STACK_SIZE),
        .cpsr = 0x50, // user mode
        .r15 = (unsigned)entry
    };

    thread* new_thread = kmalloc(sizeof(thread));
    new_thread->status = RUNNABLE;
    new_thread->state = new_state;
    new_thread->irq_stack = irq_stack;
    new_thread->svc_stack = svc_stack;

    return new_thread;
}

void thread_delete(thread* self) {
    kfree(self->irq_stack, STACK_SIZE*sizeof(unsigned));
    kfree(self->svc_stack, STACK_SIZE*sizeof(unsigned));
    kfree(self, sizeof(thread));
}
