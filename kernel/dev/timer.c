#include <pirix/irq.h>
#include <pirix/task.h>

static unsigned* timers[] = {
    (unsigned*)0x13000000,
    (unsigned*)0x13000100,
    (unsigned*)0x13000200
};

int clock;

cpu_state* clock_timer(cpu_state* state) {
    clock++;
    return state;
}

cpu_state* schedule_timer(cpu_state* state) {
    timers[1][3] = 42;
    return task_schedule(state);
}

int timer_init() {
    /*
    // initialize the clock timer
    timers[0][0] = (10000 + 128) / 256;
    timers[0][2] = 0xea;
    irq_register(5, &clock_timer);
    */

    // initialize the schedule timer
    timers[1][0] = (10000 + 128) / 256;
    timers[1][2] = 0xea;
    irq_register(6, &schedule_timer);

    return 0;
}
