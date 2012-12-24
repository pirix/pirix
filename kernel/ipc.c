#include <pirix/irq.h>
#include <pirix/ipc.h>
#include <pirix/scheduler.h>
#include <pirix/process.h>
#include <string.h>

void ipc_send(message* msg) {
    thread* src = scheduler_current_thread();
    msg->src = src->process->pid;
    while (1) {
        process* p = process_get(msg->dst);
        if (p && p->thread->status == RECEIVING) {
            memcpy(p->thread->msg, msg, sizeof(message));
            p->thread->status = RUNNABLE;
            scheduler_enqueue_thread(p->thread);
            break;
        }
        scheduler_switch();
    }
}

void ipc_recv(message* msg) {
    thread* t = scheduler_current_thread();
    message tmp_msg;
    t->msg = &tmp_msg;
    t->status = RECEIVING;
    scheduler_switch();
    memcpy(msg, &tmp_msg, sizeof(message));
}
