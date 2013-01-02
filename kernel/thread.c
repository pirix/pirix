#include <pirix/cpu.h>
#include <pirix/thread.h>
#include <pirix/process.h>
#include <pirix/kheap.h>
#include <pirix/paging.h>
#include <pirix/memory.h>
#include <string.h>

#define SVC_STACK 1024
#define THR_STACK 2

thread* thread_new(void* entry) {
    unsigned* svc_stack = kmalloc(SVC_STACK*sizeof(unsigned));

    cpu_state* new_state = (cpu_state*)(svc_stack + SVC_STACK) - 1;

    *new_state = (cpu_state) {
        .spsr = 0x50, // user mode
        .r15 = (unsigned)entry
    };

    thread* new_thread = kmalloc(sizeof(thread));
    new_thread->status = RUNNABLE;
    new_thread->state = new_state;
    new_thread->svc_stack = svc_stack;

    return new_thread;
}

void thread_init_stack(thread* self, unsigned index) {
    unsigned stack_start = 0x80000000 - (index+1)*THR_STACK*0x1000;
    unsigned stack_end = stack_start + THR_STACK*0x1000;

    for (unsigned addr = stack_start; addr < stack_end; addr += 0x1000) {
        unsigned virt = addr;
        unsigned phys = memory_alloc();
        paging_map(self->process->context, virt, phys, PTE_PERM_USER);
    }

    self->state->usr_r13 = stack_end;
    self->thr_stack = (unsigned*)stack_start;
}

void thread_delete(thread* self) {
    kfree(self->svc_stack, SVC_STACK*sizeof(unsigned));
    kfree(self, sizeof(thread));
}
