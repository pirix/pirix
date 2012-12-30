#include <pirix/irq.h>
#include <pirix/scheduler.h>
#include <pirix/timer.h>
#include <config.h>

static unsigned ticks;
static unsigned subticks;

static cpu_state* timer_tick(cpu_state* state) {
    timer_clear();

    if (!subticks) {
        subticks = TIMER_FREQ;
        ticks++;
    }
    else subticks--;

    return scheduler_schedule(state);
}

long timer_uptime() {
    return ticks;
}

void timer_init() {
    timer_setup();
    irq_register(TIMER_IRQ, &timer_tick);
}
