#include <pirix/kprint.h>
#include <pirix/frame.h>
#include <pirix/paging.h>
#include <pirix/process.h>
#include <pirix/scheduler.h>
#include <pirix/irq.h>
#include <pirix/timer.h>
#include <pirix/boot.h>
#include <pirix/memarea.h>
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

    uintptr_t addr = paging_map_kernel(module->addr, module->size);
    Elf32_Ehdr* ehdr = (Elf32_Ehdr*)addr;

    if (memcmp(ELFMAG, ehdr, SELFMAG)) {
        kprintf("invalid elf magic\n");
        paging_unmap_kernel(module->addr, module->size);
        return;
    }

    addrspace* as = addrspace_new();

    Elf32_Phdr* phdr = (Elf32_Phdr*)(addr + ehdr->e_phoff);

    for (unsigned i = 0; i < ehdr->e_phnum; i++, phdr++) {
        if (!(phdr->p_type & PT_LOAD)) continue;

        memarea* area = memarea_new(&elf_backend, MEM_READ | MEM_WRITE | MEM_EXEC);
        area->data.elf_addr = module->addr;
        area->data.elf_segment = (uintptr_t)phdr;
        addrspace_add_area(as, area);
    }

    process* proc = process_create((void*)ehdr->e_entry, as);
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
