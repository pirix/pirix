#include <pirix.h>
#include "devfs.h"

int main(int argc, const char* argv[]) {
    sys_log("device manager started.");
    devfs_run();
    return 0;
}
