static unsigned* uart_enables = (unsigned*)0x90215004;
static unsigned* uart_io = (unsigned*)0x90215040;
static unsigned* uart_ier = (unsigned*)0x90215044;
static unsigned* uart_iir = (unsigned*)0x90215048;
static unsigned* uart_lcr = (unsigned*)0x9021504c;
static unsigned* uart_mcr = (unsigned*)0x90215050;
static unsigned* uart_lsr = (unsigned*)0x90215054;
static unsigned* uart_msr = (unsigned*)0x90215058;
static unsigned* uart_scratch = (unsigned*)0x9021505c;
static unsigned* uart_cntl = (unsigned*)0x90215060;
static unsigned* uart_stat = (unsigned*)0x90215064;
static unsigned* uart_baud = (unsigned*)0x90215068;


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

void serial_init() {
    gpio_init();
    *uart_enables = 1;
    *uart_ier = 0;
    *uart_cntl = 0x2;
    *uart_lcr = 3;
    *uart_mcr = 0;
    *uart_ier = 0;
    *uart_iir = 0xc6;
    *uart_baud = 270;
}

void serial_putc(char c) {
    while (!(*uart_lsr & 0x20));
    *uart_io = c;
}
