#include <kernel/irq.h>

static irq_handler* irq_handlers[72];
static unsigned* irq_base = (unsigned*)0x9000B200;

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

int irq_init() {
    // disable all irqs
    irq_base[IRQ1_DISABLE] = 0xffffffff;
    irq_base[IRQ2_DISABLE] = 0xffffffff;
    irq_base[IRQB_DISABLE] = 0xff;

    irq_enable();
    return 0;
}

void irq_register(unsigned irq, irq_handler* handler) {
    if (irq > 71) return;
    if (irq_handlers[irq]) return;

    irq_handlers[irq] = handler;

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

void irq_unregister(unsigned irq) {
    if (irq > 71) return;

    irq_handlers[irq] = 0;

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

cpu_state* irq_handle(cpu_state* state) {
    irq_disable();

    unsigned pending = irq_base[IRQB_PENDING];

    if (pending & (1 << 9)) {
        // IRQ2 PENDING
    }

    if (pending & (1 << 8)) {
        // IRQ1 PENDING
    }

    for (int i = 0; i < 8; i++) {
        if ((pending & (1 << i)) && irq_handlers[64+i]) {
            state = irq_handlers[64+i](state);
        }
    }

    return state;
}
