#include <kernel/ipc.h>
#include <kernel/scheduler.h>
#include <kernel/process.h>

void ipc_send(message* msg) {
    while (1) {
        process* p = process_get(msg->dst);
        if (p->thread->status == RECEIVING) {
            p->thread->status == RUNNABLE;
            scheduler_enqueue_thread(p->thread);
            break;
        }
        scheduler_switch();
    }
}

void ipc_recv(message* msg) {
    thread* t = scheduler_current();
    t->status = RECEIVING;
    scheduler_switch();
}
