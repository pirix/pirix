#include <pirix/thread.h>
#include <pirix/process.h>
#include <pirix/kheap.h>
#include <pirix/paging.h>
#include <pirix/memory.h>
#include <pirix/scheduler.h>
#include <string.h>

#define SVC_STACK 1024

thread* thread_new(void* entry) {
    unsigned* svc_stack = kmalloc(SVC_STACK*sizeof(unsigned));

    registers* regs = (registers*)(svc_stack + SVC_STACK) - 1;

    // @todo !!!
    /*
    *regs = (registers) {
        .spsr = 0x50, // user mode
        .r15 = (unsigned)entry
    };
    */
    *regs = (registers) {
        .eip = (unsigned)entry,
        .cs = 0x08,
        .ss = 0x10,
        .ds = 0x10,
        .eflags = 0x202,
    };

    thread* new_thread = kmalloc(sizeof(thread));
    new_thread->state = STATE_READY;
    new_thread->regs = regs;
    new_thread->svc_stack = svc_stack;

    return new_thread;
}

void thread_set_stack(thread* self, unsigned* addr) {
    // @todo !!!
    //self->regs->usr_r13 = (unsigned)addr;
    self->regs->usr_esp = (unsigned)addr;
}

void thread_block(thread* self, thread_state state) {
    self->state = state;
}

void thread_unblock(thread* self) {
    self->state = STATE_READY;
    scheduler_enqueue_thread(self);
}

void thread_delete(thread* self) {
    kfree(self->svc_stack, SVC_STACK*sizeof(unsigned));
    kfree(self, sizeof(thread));
}
