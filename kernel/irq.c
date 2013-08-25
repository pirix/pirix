#include <pirix/irq.h>
#include <arch/cpu.h>
#include <arch/config.h>

static irq_handler* irq_handlers[IRQ_COUNT];

void irq_register(int irq, irq_handler* handler) {
    if (irq >= IRQ_COUNT) return;
    if (irq_handlers[irq]) return;

    irq_handlers[irq] = handler;
    irq_allow(irq);
}

void irq_unregister(int irq) {
    if (irq >= IRQ_COUNT) return;

    irq_handlers[irq] = 0;
    irq_disallow(irq);
}

registers* irq_run_handler(int irq, registers* regs) {
    if (irq_handlers[irq]) {
        regs = irq_handlers[irq](regs);
    }

    return regs;
}
