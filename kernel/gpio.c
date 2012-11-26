static unsigned* gpfsel1 = (unsigned*)0x90200004;
static unsigned* gpset0 = (unsigned*)0x9020001C;
static unsigned* gpclr0 = (unsigned*)0x90200028;
static unsigned* gppud = (unsigned*)0x90200094;

void gpio_init() {
    unsigned sel = *gpfsel1;
    sel &= ~(7 << 18);
    sel |= 1 << 18;
    sel &= ~(7 << 12);
    sel |= 2<<12;
    *gpfsel1 = sel;

    *gppud = 0;
}

void gpio_toggle(int sel, int state) {
    if (state) *gpclr0 = 1<<sel;
    else *gpset0 = 1<<sel;
}
