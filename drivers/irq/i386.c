#include <pirix/irq.h>
#include <arch/i386/cpu.h>
#include <arch/i386/io.h>

#define PIC1 0x20 // Master PIC
#define PIC2 0xA0 // Slave PIC
#define PIC1_DATA (PIC1+1)
#define PIC2_DATA (PIC2+1)
#define PIC_EOI 0x20 // end of interrupt
#define IRQ_BASE 0x20

// check a PIC is responsible for this IRQ
static inline int irq_in_pic_range(int irq) {
   return irq >= IRQ_BASE && irq <= IRQ_BASE+15;
}

void irq_init() {
    // start initialization
    outb(PIC1, 0x11);
    outb(PIC2, 0x11);

    // set IRQ base numbers for each PIC
    outb(PIC1_DATA, IRQ_BASE);
    outb(PIC2_DATA, IRQ_BASE+8);

    // use IRQ number 2 to relay IRQs from the slave PIC
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // finish initialization
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // mask all IRQs
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);

    irq_allow(0x2c);
}

void irq_allow(int irq) {
    if (!irq_in_pic_range(irq)) return;

    // find the concerned PIC
    int pic_data = irq < (IRQ_BASE+8) ? PIC1_DATA : PIC2_DATA;

    // clear the bit
    uint8_t mask = inb(pic_data);
    mask &= ~(1 << (irq-IRQ_BASE));
    outb(pic_data, mask);
}

void irq_disallow(int irq) {
    if (!irq_in_pic_range(irq)) return;

    // find the concerned PIC
    int pic_data = irq < (IRQ_BASE+8) ? PIC1_DATA : PIC2_DATA;

    // set the bit
    uint8_t mask = inb(pic_data);
    mask |= 1 << (irq-IRQ_BASE);
    outb(pic_data, mask);
}

registers* irq_handle(registers* regs) {
    // if this interrupt came from a PIC, send an end of interrupt
    // command to the concerned PICs
    if (irq_in_pic_range(regs->irq)) {
        if (regs->irq >= IRQ_BASE+8) {
            outb(PIC2, PIC_EOI);
        }
        outb(PIC1, PIC_EOI);
    }

    return irq_run_handler(regs->irq, regs);
}
