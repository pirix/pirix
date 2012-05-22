#include <ix/task.h>
#include <ix/irq.h>
#include <ix/memory.h>
#include <ix/paging.h>
#include <ix/kprint.h>

void panic() {
    kputs("\nKernel Panic!\n");
    for (;;);
}

void main() {
    kputs("IX VERSION 0.1 BOOTING...\n\n");

    kputs(":: Init Memory\n");
    memory_init();

    kputs(":: Init Paging\n");
    paging_init();

    initrd_load();

    kputs(":: Init Interrupts\n");
    irq_init();

    kputs(":: Start Timer\n");
    timer_init();

    for (;;);
}
