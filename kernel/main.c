#include <pirix/kprint.h>
#include <pirix/memory.h>
#include <pirix/paging.h>
#include <pirix/process.h>
#include <pirix/scheduler.h>
#include <pirix/irq.h>
#include <pirix/timer.h>
#include <pirix/boot.h>
#include <arch/cpu.h>
#include <arch/config.h>
#include <config.h>

void panic(const char* cause) {
    kputs("\n\nKERNEL PANIC!");
    if (cause) kprintf(" (%s)", cause);
    kputs("\nPlease reboot the system.");
    for (;;);
}

void exception(registers* regs) {
    kputs("\n\nEXCEPTION!\n");
    panic(0);
}

void main(boot_info* info) {
    kputs("Pirix " VERSION " - " PLATFORM_NAME " " BUILD_TYPE " build\n");

    memory_init();
    paging_init();
    irq_init();
    timer_init();
    process_init();
    scheduler_init();
    irq_enable();

    for (;;);
}
