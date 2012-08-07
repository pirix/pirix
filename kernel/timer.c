#include <pirix/irq.h>
#include <pirix/task.h>

static unsigned* timers[] = {
    (unsigned*)0x2000B400,
};

cpu_state* timer_tick(cpu_state* state) {
    // clear interrupt
    timers[0][3] = 42;
    kputs("tick\n");
    return state;
}

int timer_init() {
    // initialize the schedule timer
    timers[0][2] = 0b1010100000;

    irq_register(64, &timer_tick);

    return 0;
}
