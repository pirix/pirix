#include <arch/arm/cpu.h>

static unsigned* irq_base = (unsigned*)0x94000000;

void irq_setup() {
    irq_base[3] = 0xffffffff;
}

void irq_allow(unsigned irq) {
    irq_base[2] |= 1 << irq;
}

void irq_disallow(unsigned irq) {
    // todo
}

unsigned irq_find(registers* regs) {
    unsigned status = irq_base[0];

    for (int i = 0; i < 32; i++) {
        if (status & (1 << i)) {
            return i;
        }
    }

    return 0;
}
