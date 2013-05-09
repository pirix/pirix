#include <arch/i386.h>

void irq_setup() {
    outb(0x20, 0x11);
    outb(0x21, 0x20);
    outb(0x21, 0x04);
    outb(0x21, 0x01);

    outb(0xa0, 0x11);
    outb(0xa1, 0x28);
    outb(0xa1, 0x02);
    outb(0xa1, 0x01);
}

void irq_allow(unsigned irq) {

}

void irq_disallow(unsigned irq) {

}

unsigned irq_find(registers* regs) {
    if (regs->irq >= 0x20 && regs->irq <= 0x2f) {
        if (regs->irq >= 0x28) {
            outb(0xa0, 0x20);
        }
        outb(0x20, 0x20);
    }
    return regs->irq;
}
