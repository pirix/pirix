#include <pirix/kprint.h>
#include <pirix/frame.h>
#include <pirix/paging.h>
#include <pirix/process.h>
#include <pirix/scheduler.h>
#include <pirix/irq.h>
#include <pirix/timer.h>
#include <pirix/boot.h>
#include <arch/cpu.h>
#include <arch/config.h>
#include <elf.h>
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

static void module_load(boot_module* module) {
    kprintf("Loading %s\n", module->args);

    addrspace* as = addrspace_new();
    process* proc = process_create((void*)0x7fffff00, as);

    uintptr_t addr = paging_map_kernel(module->addr, module->size);

    Elf32_Ehdr* ehdr = (Elf32_Ehdr*)addr;

    if (memcmp(ELFMAG, ehdr, SELFMAG)) {
        kprintf("invalid elf magic\n");
    }
    else {
        kprintf("valid elf magic\n");
    }
}

static void module_init(boot_info* info) {
    kprintf("Loading modules...\n");
    for (int i = 0; i < info->mod_count; i++) {
        module_load(&info->modules[i]);
    }
}

void main(boot_info* info) {
    kputs("Pirix " VERSION " - " PLATFORM_NAME " " BUILD_TYPE " build\n");

    frame_init();
    paging_init();
    irq_init();
    timer_init();
    process_init();
    scheduler_init();
    module_init(info);
    irq_enable();

    for (;;);
}
