#include <pirix/ipc.h>
#include <servers/system.h>
#include <stdio.h>
#include "pathmgr.h"

/*
void handle() {
    switch (5) {
    case PATHMGR_RESOLVE:
        break;
    case PATHMGR_ATTACH:
        pathmgr_attach("/dev", 5);
        break;
    case PATHMGR_DETACH:
        break;
    }
}*/

int main(int argc, char* argv[]) {
    puts("Hello World from system!");
    //sys_log("system process started");

    //pathmgr_init();

    //int chan = ipc_listen();

    while (1) {
        //union system_msg buffer;

        //int recvid = ipc_receive(chan, &MSG_BUF_LIN(&buffer, sizeof(buffer)));

        //ipc_reply(recvid, 1337, 0);
    }

    return 0;
}
