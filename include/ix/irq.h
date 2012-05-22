#ifndef IX_IRQ_H
#define IX_IRQ_H

#include <ix/cpu.h>

typedef cpu_state* (irq_handler)(cpu_state*);

void irq_init();
void irq_enable();
void irq_register(unsigned irq, irq_handler* handler);

#endif
