#include <arch/arm/cpu.h>

static uint32_t* irq_base = (uint32_t*)0x94000000;

void irq_init() {
    irq_base[3] = 0xffffffff;
}

void irq_allow(int irq) {
    irq_base[2] |= 1 << irq;
}

void irq_disallow(int irq) {
    // TODO
}

registers* irq_handle(registers* regs) {
    uint32_t pending = irq_base[0];

    for (int i = 0; i < 32; i++) {
        if (pending & (1 << i)) {
            return irq_run_handler(i, regs);
        }
    }

    return 0;
}
