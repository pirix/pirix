#pragma once

#include <pirix/cpu.h>

typedef cpu_state* (irq_handler)(cpu_state*);

int irq_init();
void irq_enable();
void irq_disable();

void irq_register(unsigned irq, irq_handler* handler);
void irq_unregister(unsigned irq);
