#include <pirix/irq.h>
#include <pirix/ipc.h>
#include <pirix/scheduler.h>
#include <pirix/process.h>
#include <string.h>

int ipc_send(int dst, message* msg) {
    thread* src = scheduler_current_thread();
    msg->src = src->process->pid;

    while (1) {
        process* p = process_get(dst);

        if (p) {
            for (int i = 0; i < MAX_THREADS; i++) {
                thread* t = p->threads[i];

                if (!t) continue;
                if (t->status != RECEIVING) continue;
                if (t->msg->src != ANY_PID && t->msg->src != msg->src) continue;

                memcpy(p->threads[i]->msg, msg, sizeof(message));
                p->threads[i]->status = RUNNABLE;
                scheduler_enqueue_thread(p->threads[i]);
                return 0;
            }
        }

        scheduler_switch();
    }

    return 0;
}

int ipc_recv(int src, message* msg) {
    thread* t = scheduler_current_thread();

    message tmp_msg;
    t->msg = &tmp_msg;
    t->msg->src = src;
    t->status = RECEIVING;

    scheduler_switch();
    memcpy(msg, &tmp_msg, sizeof(message));
    return 0;
}

int ipc_call(int dst, message* msg) {
    int res = ipc_send(dst, msg);
    if (res != 0) return res;
    return ipc_recv(dst, msg);
}
