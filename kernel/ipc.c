#include <pirix/irq.h>
#include <pirix/ipc.h>
#include <pirix/scheduler.h>
#include <pirix/process.h>
#include <string.h>

int ipc_send(int dest, message* msg) {
    thread* src = scheduler_current_thread();
    msg->src = src->process->pid;

    while (1) {
        process* p = process_get(dest);
        if (p && p->threads[0] && p->threads[0]->status == RECEIVING) {
            memcpy(p->threads[0]->msg, msg, sizeof(message));
            p->threads[0]->status = RUNNABLE;
            scheduler_enqueue_thread(p->threads[0]);
            break;
        }
        scheduler_switch();
    }

    return 0;
}

int ipc_recv(message* msg) {
    thread* t = scheduler_current_thread();
    message tmp_msg;
    t->msg = &tmp_msg;
    t->status = RECEIVING;
    scheduler_switch();
    memcpy(msg, &tmp_msg, sizeof(message));
    return 0;
}

int ipc_call(int dest, message* msg) {
    int res = ipc_send(dest, msg);
    if (res != 0) return res;
    return ipc_recv(msg);
}
