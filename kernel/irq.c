#include <pirix/irq.h>
#include <arch.h>
#include <config.h>

static irq_handler* irq_handlers[IRQ_COUNT];

void irq_setup();
void irq_allow(unsigned irq);
void irq_disallow(unsigned irq);
unsigned irq_find(registers* regs);

void irq_init() {
    irq_setup();
    irq_enable();
}

void irq_register(unsigned irq, irq_handler* handler) {
    if (irq > IRQ_COUNT-1) return;
    if (irq_handlers[irq]) return;

    irq_handlers[irq] = handler;
    irq_allow(irq);
}

void irq_unregister(unsigned irq) {
    if (irq > IRQ_COUNT-1) return;

    irq_handlers[irq] = 0;
    irq_disallow(irq);
}

registers* irq_handle(registers* regs) {
    unsigned irq = irq_find(regs);

    kprintf("interrupt %i\n", irq);

    if (irq_handlers[irq]) {
        regs = irq_handlers[irq](regs);
    }

    return regs;
}
