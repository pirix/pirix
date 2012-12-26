#include <stdio.h>
#include <stdlib.h>
#include "pirix.h"
#include "pirix/ipc.h"

int main(int argc, const char* argv[]) {
    sys_log("device manager started.");
    message msg;
    msg.dst = 0;
    msg.op = 42;
    msg.data = 23;
    sys_send(&msg);
    sys_log("message sent!");
    for (;;);
    return 0;
}
