#include "kernel/syscall.h"

int main(int argc, char* argv[]) {
    sys_log("virtual file system started.");
    for (;;);
    while (1) {
        for (int i = 0; i < 10000000; i++);
        sys_log("vfs is running");
    }
    return 0;
}
