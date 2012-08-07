#include <pirix/kprint.h>
#include <pirix/timer.h>
#include <pirix/irq.h>
#include <pirix/memory.h>
#include <pirix/paging.h>
#include <pirix/boot.h>

void panic() {
    kputs("\n\nKERNEL PANIC!\n");
    kputs("Please reboot the system.");
    for (;;);
}

static void init(const char* name, int (*handler)()) {
    kprintf(":: %s \t", name);
    int res = handler();
    if (res) kputs("[fail]\n");
    else kputs("[ok]\n");
}

void main() {
    gpio_init();

    serial_init();

    gpio_toggle(16, 1);

    kputs("PIRIX VERSION 0.1 BOOTING...\n\n");

    init("memory", &memory_init);
    init("paging", &paging_init);
    init("irqs", &irq_init);
    init("timer", &timer_init);

    kputs("\nReady.");

    for (;;);
}
