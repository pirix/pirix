#include <kernel/kprint.h>
#include <kernel/timer.h>
#include <kernel/irq.h>
#include <kernel/memory.h>
#include <kernel/paging.h>
#include <kernel/process.h>
#include <kernel/boot.h>

void panic() {
    kputs("\n\nKERNEL PANIC!\n");
    kputs("Please reboot the system.");
    for (;;);
}

void exception(cpu_state* state) {
    kputs("\n\nEXCEPTION!\n");
    kprintf("r01: %p\t r02: %p\t r03: %p\n", state->r1, state->r2, state->r3);
    kprintf("r04: %p\t r05: %p\t r06: %p\n", state->r4, state->r5, state->r6);
    kprintf("r07: %p\t r08: %p\t r09: %p\n", state->r7, state->r8, state->r9);
    kprintf("r10: %p\t r11: %p\t r12: %p\n", state->r10, state->r11, state->r12);
    kprintf("usr_r13: %p\t usr_r14: %p\n", state->usr_r13, state->usr_r14);
    kprintf("svc_r13: %p\t svc_r14: %p\n", state->svc_r13, state->svc_r14);
    kprintf("r15: %p\n", state->r15);
    panic();
}

static void init(const char* name, int (*handler)()) {
    kprintf(":: %s", name);
    int res = handler();
    if (res) kputs("\t[fail]\n");
    else kputs("\t[ok]\n");
}

static int modules_init() {
    extern unsigned* kernel_end;
    boot_header* boothdr = (boot_header*)&kernel_end;

    if (boothdr->module_count > BOOT_MAX_MODULES) {
        return 1;
    }

    for (unsigned int i = 0; i < boothdr->module_count; i++) {
        boot_module* mod = boothdr->modules + i;

        paging_context* context = paging_create_context();

        // map memory
        for (unsigned i = 0; i < mod->size; i += 0x1000) {
            paging_map(context, 0x8000+i, mod->addr+i, PTE_PERM_USER);
        }

        // map stack
        paging_map(context, 0x7ffff000, memory_alloc(), PTE_PERM_USER);

        process_create((void*)mod->entry, context);
    }

    return 0;
}

void main() {
    serial_init();

    kputs("PIRIX VERSION 0.1 BOOTING...\n\n");

    init("memory", &memory_init);
    init("paging", &paging_init);
    init("modules", &modules_init);
    init("irqs", &irq_init);
    init("timer", &timer_init);

    for (;;);
}
