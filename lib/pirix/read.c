#include <pirix/ipc.h>

int read(int fd, char* buf, int len) {
    //io_read_msg msg;

    //msg.type = IO_READ;
    //msg.size = len;
    const char msg[] = "READ";

    msg_buffer out = MSG_BUF_LIN(msg, sizeof(msg));
    msg_buffer in = MSG_BUF_LIN(buf, len);

    return ipc_send(fd, &out, &in);
}
