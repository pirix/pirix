#include <pirix/irq.h>
#include <pirix/ipc.h>
#include <pirix/scheduler.h>
#include <pirix/process.h>
#include <pirix/kheap.h>
#include <string.h>

int ipc_listen() {
    process* proc = scheduler_current_thread()->process;
    if (proc->chan) return -1;

    channel* chan = kmalloc(sizeof(channel));
    chan->proc = proc;
    proc->chan = chan;
    vector_init(&chan->reply);

    semaphore_init(&chan->recv_sem, 0);
    semaphore_init(&chan->send_sem, 1);

    return 0;
}

int ipc_connect(int pid) {
    thread* t = scheduler_current_thread();
    process* proc = t->process;
    process* dest = process_get(pid);

    if (!dest || !dest->chan) return -1;

    // create connect object
    connect* conn = kmalloc(sizeof(connect));
    int cid = vector_add(&proc->fds, conn);
    conn->chan = dest->chan;
    conn->cid = cid;
    conn->owner = t;

    return cid;
}

int ipc_send(int cid, message* msg) {
    thread* t = scheduler_current_thread();
    process* proc = t->process;
    connect* conn = vector_get(&proc->fds, cid);
    channel* chan = conn->chan;

    if (!conn) return -1;

    semaphore_wait(&chan->send_sem);

    memcpy(&chan->msg_buffer, msg, sizeof(message));
    chan->reply_id = vector_add(&chan->reply, conn);

    semaphore_signal(&conn->chan->recv_sem);

    // set the current thread to receiving
    thread_block(t, STATE_REPLY);
    scheduler_switch();

    memcpy(msg, &conn->msg_buffer, sizeof(message));

    return 0;
}

int ipc_recv(message* msg) {
    thread* t = scheduler_current_thread();
    channel* chan = t->process->chan;

    if (!chan) return -1;

    semaphore_wait(&chan->recv_sem);

    memcpy(msg, &chan->msg_buffer, sizeof(message));
    int rid = chan->reply_id;

    semaphore_signal(&chan->send_sem);

    return rid;
}

int ipc_reply(int rid, message* msg) {
    thread* t = scheduler_current_thread();
    channel* chan = t->process->chan;

    connect* conn = vector_get(&chan->reply, rid);
    vector_set(&chan->reply, rid, 0);

    memcpy(&conn->msg_buffer, msg, sizeof(message));

    thread_unblock(conn->owner);

    return 0;
}
