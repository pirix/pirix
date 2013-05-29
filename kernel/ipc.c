#include <pirix/irq.h>
#include <pirix/ipc.h>
#include <pirix/scheduler.h>
#include <pirix/process.h>
#include <pirix/kheap.h>
#include <pirix/paging.h>
#include <sys/uio.h>
#include <string.h>

static unsigned iovec_sum(struct iovec* iov, int count) {
    unsigned size = 0;
    for (int i = 0; i < count; i++) {
        size += (iov+i)->iov_len;
    }
    return size;
}

static void iovec_read(struct iovec* iov, int parts, char* buffer) {
    for (int i = 0; i < parts; i++) {
        memcpy(buffer, (iov+i)->iov_base, (iov+i)->iov_len);
        buffer += (iov+i)->iov_len;
    }
}

static void iovec_write(struct iovec* iov, int parts, char* buffer, unsigned size) {
    for (int i = 0; i < parts && size > 0; i++) {
        size_t len = (iov+i)->iov_len;
        if (len > size) len = size;
        memcpy((iov+i)->iov_base, buffer, len);
        buffer += len;
        size -= len;
    }
}

static void read_message(msg_buffer* msg_buf, unsigned* size, char** buffer) {
    switch (msg_buf->flags & MSG_TYPE) {
    case MSG_LIN: {
        *size = msg_buf->size;
        *buffer = kmalloc(*size);
        memcpy(*buffer, msg_buf->data, *size);
        break;
    }

    case MSG_VEC: {
        struct iovec* iov = msg_buf->data;
        *size = iovec_sum(iov, msg_buf->size);
        *buffer = kmalloc(*size);
        iovec_read(iov, msg_buf->size, *buffer);
        break;
    }
    }
}

static void write_message(msg_buffer* msg_buf, unsigned* size, char** buffer) {
    switch (msg_buf->flags & MSG_TYPE) {
    case MSG_LIN: {
        unsigned tsize = (*size) > msg_buf->size ? msg_buf->size : *size;
        memcpy(msg_buf->data, *buffer, tsize);
        break;
    }

    case MSG_VEC: {
        struct iovec* iov = msg_buf->data;
        iovec_write(iov, msg_buf->size, *buffer, *size);
        break;
    }
    }

    kfree(*buffer, *size);
    *size = 0;
    *buffer = 0;
}

int ipc_listen() {
    process* proc = scheduler_current_thread()->process;

    channel* chan = kmalloc(sizeof(channel));
    chan->proc = proc;
    proc->chan = chan;
    vector_init(&chan->reply, 4);

    semaphore_init(&chan->recv_sem, 0);
    semaphore_init(&chan->send_sem, 1);

    int chanid = vector_add(&proc->chans, chan);

    return chanid;
}

int ipc_close(int chanid) {
    process* proc = scheduler_current_thread()->process;

    channel* chan = vector_get(&proc->chans, chanid);

    vector_free(&chan->reply);
    kfree(chan, sizeof(channel));

    vector_set(&proc->chans, chanid, 0);

    return 0;
}

int ipc_connect(int pid, int chanid) {
    thread* t = scheduler_current_thread();
    process* proc = t->process;
    process* dest = process_get(pid);

    if (!dest) return -1;

    channel* chan = vector_get(&dest->chans, chanid);

    if (!chan) return -1;

    // create connect object
    connect* conn = kmalloc(sizeof(connect));
    int cid = vector_add(&proc->fds, conn);
    conn->chan = dest->chan;
    conn->cid = cid;

    return cid;
}

int ipc_disconnect(int cid) {
    process* proc = scheduler_current_thread()->process;

    connect* conn = vector_get(&proc->fds, cid);

    if (!conn) return -1;

    kfree(conn, sizeof(connect));
    vector_set(&proc->fds, cid, 0);

    return 0;
}

int ipc_send(int cid, msg_buffer* send_buf, msg_buffer* recv_buf) {
    thread* t = scheduler_current_thread();
    process* proc = t->process;
    connect* conn = vector_get(&proc->fds, cid);
    channel* chan = conn->chan;

    if (!conn) return -1;

    conn->owner = t;

    semaphore_wait(&chan->send_sem);

    read_message(send_buf, &chan->size, &chan->buffer);
    chan->recvid = vector_add(&chan->reply, conn);

    semaphore_signal(&conn->chan->recv_sem);

    // set the current thread to receiving
    thread_block(t, STATE_REPLY);
    scheduler_switch();

    if (conn->size) {
        write_message(recv_buf, &conn->size, &conn->buffer);
    }

    return conn->status;
}

int ipc_receive(int chanid, msg_buffer* recv_buf) {
    thread* t = scheduler_current_thread();
    channel* chan = vector_get(&t->process->chans, chanid);

    if (!chan) return -1;

    semaphore_wait(&chan->recv_sem);

    write_message(recv_buf, &chan->size, &chan->buffer);

    int recvid = chan->recvid;

    semaphore_signal(&chan->send_sem);

    return recvid;
}

int ipc_reply(int recvid, int status, msg_buffer* send_buf) {
    thread* t = scheduler_current_thread();
    channel* chan = t->process->chan;

    connect* conn = vector_get(&chan->reply, recvid);
    vector_set(&chan->reply, recvid, 0);

    conn->status = status;

    if (send_buf) {
        read_message(send_buf, &conn->size, &conn->buffer);
    }

    thread_unblock(conn->owner);

    return 0;
}
