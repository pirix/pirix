static unsigned* timers[] = {
    (unsigned*)0x93000000,
    (unsigned*)0x93000100,
    (unsigned*)0x93000200
};

void timer_setup() {
    timers[1][0] = (10000 + 128) / 256;
    timers[1][2] = 0xea;
}

void timer_clear() {
    timers[1][3] = 42;
}
