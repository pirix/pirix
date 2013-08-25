#include <pirix/irq.h>
#include <arch/arm/cpu.h>

static uint32_t* irq_base = (uint32_t*)0x9000B200;

#define IRQB_PENDING 0
#define IRQ1_PENDING 1
#define IRQ2_PENDING 2
#define FIQ_CONTROL 3
#define IRQ1_ENABLE 4
#define IRQ2_ENABLE 5
#define IRQB_ENABLE 6
#define IRQ1_DISABLE 7
#define IRQ2_DISABLE 8
#define IRQB_DISABLE 9

void irq_init() {
    irq_base[IRQ1_DISABLE] = 0xffffffff;
    irq_base[IRQ2_DISABLE] = 0xffffffff;
    irq_base[IRQB_DISABLE] = 0xff;
}

void irq_allow(int irq) {
    if (irq < 32) {
        irq_base[IRQ1_ENABLE] |= 1 << irq;
    }
    else if (irq < 64) {
        irq_base[IRQ2_ENABLE] |= 1 << (irq-32);
    }
    else {
        irq_base[IRQB_ENABLE] |= 1 << (irq-64);
    }
}

void irq_disallow(int irq) {
    if (irq < 32) {
        irq_base[IRQ1_DISABLE] |= 1 << irq;
    }
    else if (irq < 64) {
        irq_base[IRQ2_DISABLE] |= 1 << (irq-32);
    }
    else {
        irq_base[IRQB_DISABLE] |= 1 << (irq-64);
    }
}

registers* irq_handle(registers* regs) {
    uint32_t pending = irq_base[IRQB_PENDING];

    if (pending & (1 << 9)) {
        // IRQ2 PENDING
    }

    if (pending & (1 << 8)) {
        // IRQ1 PENDING
    }

    for (int i = 0; i < 8; i++) {
        if (pending & (1 << i)) {
            return irq_run_handler(64 + i, regs);
        }
    }

    return regs;
}
