#include <pirix/irq.h>
#include <pirix/task.h>

static unsigned* timers[] = {
    (unsigned*)0x13000000,
    (unsigned*)0x13000100,
    (unsigned*)0x13000200
};

static int clock;

cpu_state* schedule_timer(cpu_state* state) {
    timers[1][3] = 42;
    return task_schedule(state);
}

cpu_state* clock_timer(cpu_state* state) {
    timers[2][3] = 42;
    clock++;
    kprintf("%i\n", clock);
    return state;
}

int timer_init() {
    // initialize the schedule timer
    timers[1][0] = (10000 + 128) / 256;
    timers[1][2] = 0xea;
    irq_register(6, &schedule_timer);

    // initialize the clock timer
    timers[2][0] = 1000000;
    timers[2][2] = 0xe2;
    irq_register(7, &clock_timer);

    return 0;
}
