#pragma once

#include <arch.h>

typedef registers* (irq_handler)(registers*);

void irq_init();
void irq_register(unsigned irq, irq_handler* handler);
void irq_unregister(unsigned irq);
