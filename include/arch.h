#pragma once

typedef struct thread thread;

void arch_switch_context(thread* t);
void arch_switch_task(registers* regs);