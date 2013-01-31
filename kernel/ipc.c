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

    return 0;
}

int ipc_connect(int pid) {
    thread* thrd = scheduler_current_thread();
    process* proc = thrd->process;
    process* dest = process_get(pid);

    if (!dest->chan) return -1;

    connect* conn = kmalloc(sizeof(connect));
    int cid = vector_add(&proc->fds, conn);
    conn->chan = dest->chan;
    conn->cid = cid;

    kprintf("connected!\n");

    return cid;
}

int ipc_send(int cid, message* msg) {
    thread* thrd = scheduler_current_thread();
    process* proc = thrd->process;
    connect* conn = vector_get(&proc->fds, cid);

    if (!conn) return -1;

    memcpy(&conn->chan->msg, msg, sizeof(message));

    kprintf("sent!\n");

    thread_block(thrd, STATE_RECV);

    scheduler_switch();

    kprintf("got reply!\n");

    return 0;
}

int ipc_recv(message* msg) {
    thread* t = scheduler_current_thread();

    thread_block(t, STATE_RECV);

    scheduler_switch();

    return 0;
}

int ipc_reply(int cid, message* msg) {
    kprintf("reply!");

    return 0;
}
