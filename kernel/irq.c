#include <pirix/irq.h>
#include <config.h>

static irq_handler* irq_handlers[IRQ_COUNT];

void irq_setup();
void irq_allow(unsigned irq);
void irq_disallow(unsigned irq);
unsigned irq_find();

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

cpu_state* irq_handle(cpu_state* state) {
    irq_disable();

    unsigned irq = irq_find();

    if (irq_handlers[irq]) {
        state = irq_handlers[irq](state);
    }

    return state;
}
