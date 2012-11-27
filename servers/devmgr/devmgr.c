#include "kernel/syscall.h"

int main(int argc, const char* argv[]) {
    sys_log("device manager started.");
    for (;;);
    return 0;
}
