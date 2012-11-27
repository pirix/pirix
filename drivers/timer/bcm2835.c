static unsigned* timer = (unsigned*)0x9000B400;

void timer_setup() {
    timer[0] = 50;
    timer[2] = 0b1010101000;
    timer[7] = 1000;
}

void timer_clear() {
    timer[3] = 42;
}
