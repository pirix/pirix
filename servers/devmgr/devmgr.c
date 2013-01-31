#include <pirix.h>
#include <stdio.h>
#include "devfs.h"

int main(int argc, char* argv[]) {
    sys_log("device manager started.");
    //puts("Hello!");
    //printf("coid: %i\n", coid);
    devfs_run();
    return 0;
}
