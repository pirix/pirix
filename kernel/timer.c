#include <pirix/irq.h>
#include <pirix/scheduler.h>
#include <pirix/timer.h>
#include <arch/config.h>

static unsigned ticks;
static unsigned subticks;

static registers* timer_tick(registers* regs) {
    timer_clear();

    if (!subticks) {
        subticks = TIMER_FREQ;
        ticks++;
    }
    else subticks--;

    return scheduler_schedule(regs);
}

long timer_uptime() {
    return ticks;
}

void timer_init() {
    timer_setup();
    irq_register(TIMER_IRQ, &timer_tick);
}
