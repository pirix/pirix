#include <pirix/kprint.h>
#include <pirix/timer.h>
#include <pirix/irq.h>
#include <pirix/memory.h>
#include <pirix/paging.h>
#include <pirix/process.h>
#include <pirix/scheduler.h>
#include <pirix/boot.h>
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

static void modules_init() {
    extern unsigned* kernel_end;
    boot_header* boothdr = (boot_header*)&kernel_end;

    if (boothdr->module_count > BOOT_MAX_MODULES) {
        panic("too many modules");
    }

    for (unsigned int i = 0; i < boothdr->module_count; i++) {
        boot_module* mod = boothdr->modules + i;

        kprintf("loading %s...\n", mod->name);

        paging_context context = paging_create_context();

        // map memory
        for (unsigned i = 0; i < mod->size; i += 0x1000) {
            paging_map(context, 0x8000+i, mod->addr+i, PAGE_PERM_USER);
        }

        process_create((void*)mod->entry, context);
    }
}

void main() {
    serial_init();

    kputs("PIRIX VERSION " VERSION " BOOTING...\n");
    kputs(PLATFORM_NAME " " BUILD_TYPE " build (" BUILD_TIME ")\n\n");

    memory_init();
    paging_init();
    process_init();
    scheduler_init();
    modules_init();
    irq_init();
    timer_init();

    for (;;);
}
