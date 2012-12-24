#include <pirix/cpu.h>
#include <pirix/thread.h>
#include <pirix/kheap.h>
#include <string.h>

#define STACK_SIZE 1024

thread* thread_new(void* entry) {
    unsigned* svc_stack = kmalloc(STACK_SIZE*sizeof(unsigned));

    cpu_state* new_state = (cpu_state*)(svc_stack + STACK_SIZE) - 1;

    *new_state = (cpu_state) {
        .usr_r13 = 0x80000000,
        .spsr = 0x50, // user mode
        .r15 = (unsigned)entry
    };

    thread* new_thread = kmalloc(sizeof(thread));
    new_thread->status = RUNNABLE;
    new_thread->state = new_state;
    new_thread->svc_stack = svc_stack;

    return new_thread;
}

void thread_delete(thread* self) {
    kfree(self->svc_stack, STACK_SIZE*sizeof(unsigned));
    kfree(self, sizeof(thread));
}
