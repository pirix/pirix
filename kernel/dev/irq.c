#include <pirix/irq.h>

static unsigned int* ic_regs = (unsigned int*)0x14000000;
static irq_handler* irq_handlers[32];

int irq_init() {
    // unmap all irqs
    ic_regs[3] = 0xffffffff;
    irq_enable();
    return 0;
}

void irq_register(unsigned irq, irq_handler* handler) {
    if (irq >= 32) return;
    if (irq_handlers[irq]) return;

    irq_handlers[irq] = handler;
    ic_regs[2] |= 1 << irq;
}

cpu_state* irq_handle(cpu_state* state) {
    unsigned irq = 0;
    unsigned status = ic_regs[0];

    while (status) {
      if (status & 1) {
          if (irq_handlers[irq]) {
              state = irq_handlers[irq](state);
          }
      }

      irq++;
      status >>= 1;
    }

    return state;
}
