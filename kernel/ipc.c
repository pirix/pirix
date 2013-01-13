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
            for (int i = 0; i < p->threads.bounds; i++) {
                thread* t = vector_get(&p->threads, i);

                if (!t) continue;
                if (t->state != STATE_RECV) continue;
                if (t->msg->src != ANY_PID && t->msg->src != msg->src) continue;

                memcpy(t->msg, msg, sizeof(message));

                thread_unblock(t);
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

    thread_block(t, STATE_RECV);

    scheduler_switch();
    memcpy(msg, &tmp_msg, sizeof(message));
    return 0;
}

int ipc_call(int dst, message* msg) {
    int res = ipc_send(dst, msg);
    if (res != 0) return res;
    return ipc_recv(dst, msg);
}
