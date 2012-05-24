#include <pirix/kprint.h>
#include <pirix/task.h>
#include <pirix/irq.h>
#include <pirix/memory.h>
#include <pirix/paging.h>
#include <pirix/kheap.h>
#include <pirix/initrd.h>

void panic() {
    kputs("\nKernel Panic!\n");
    for (;;);
}

void main() {
    kputs("PIRIX VERSION 0.1 BOOTING...\n\n");

    video_init();

    kputs(":: Init Memory\n");
    memory_init();

    kputs(":: Init Paging\n");
    paging_init();

    kputs(":: Load Initrd\n");
    initrd_load();

    kputs(":: Init Interrupts\n");
    irq_init();

    kputs(":: Start Timer\n");
    timer_init();

    for (;;);
}
