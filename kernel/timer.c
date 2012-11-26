#include <kernel/irq.h>
#include <kernel/process.h>

static unsigned* timers[] = {
    (unsigned*)0x9000B400,
};

static unsigned clock;

cpu_state* timer_tick(cpu_state* state) {
    // clear interrupt
    timers[0][3] = 42;
    clock++;
    return schedule(state);
}

int timer_init() {
    // initialize the schedule timer
    timers[0][0] = 50;
    timers[0][2] = 0b1010101000;
    timers[0][7] = 1000;

    irq_register(64, &timer_tick);

    return 0;
}
