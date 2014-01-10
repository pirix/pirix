#include <pirix/thread.h>
#include <pirix/process.h>
#include <pirix/kheap.h>
#include <pirix/paging.h>
#include <pirix/frame.h>
#include <pirix/scheduler.h>
#include <pirix/string.h>
#include <pirix/vector.h>
#include <arch/cpu.h>

#define SVC_STACK 1024

thread* thread_new(void* entry) {
    uintptr_t svc_stack = (uintptr_t)kmalloc(SVC_STACK*sizeof(uint32_t));

    registers* regs = (registers*)(svc_stack + SVC_STACK) - 1;

    registers_init(regs, entry);

    thread* new_thread = kmalloc(sizeof(thread));
    new_thread->state = STATE_READY;
    new_thread->regs = regs;
    new_thread->svc_stack = svc_stack;

    return new_thread;
}

void thread_set_stack(thread* self, uintptr_t addr) {
    registers_set_stack(self->regs, addr);
}

void thread_block(thread* self, thread_state state) {
    self->state = state;
}

void thread_unblock(thread* self) {
    self->state = STATE_READY;
    scheduler_enqueue_thread(self);
}

void thread_delete(thread* self) {
    kfree((void*)self->svc_stack, SVC_STACK*sizeof(uint32_t));
    kfree(self, sizeof(thread));
}
