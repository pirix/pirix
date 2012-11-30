#include <kernel/irq.h>
#include <kernel/scheduler.h>
#include <kernel/timer.h>
#include <config.h>

static unsigned clock;

static cpu_state* timer_tick(cpu_state* state) {
    timer_clear();
    clock++;
    return scheduler_schedule(state);
}

int timer_init() {
    timer_setup();
    irq_register(TIMER_IRQ, &timer_tick);
    return 0;
}
