static unsigned* uart_enables = (unsigned*)0x20215004;
static unsigned* uart_io = (unsigned*)0x20215040;
static unsigned* uart_ier = (unsigned*)0x20215044;
static unsigned* uart_iir = (unsigned*)0x20215048;
static unsigned* uart_lcr = (unsigned*)0x2021504c;
static unsigned* uart_mcr = (unsigned*)0x20215050;
static unsigned* uart_lsr = (unsigned*)0x20215054;
static unsigned* uart_msr = (unsigned*)0x20215058;
static unsigned* uart_scratch = (unsigned*)0x2021505c;
static unsigned* uart_cntl = (unsigned*)0x20215060;
static unsigned* uart_stat = (unsigned*)0x20215064;
static unsigned* uart_baud = (unsigned*)0x20215068;

void serial_init() {
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

void serial_puts(const char* str) {
    while (*str) {
        serial_putc(*str++);
    }
}
