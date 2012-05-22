#define SERIAL_BUFFER_FULL (1 << 5)

static char* dr = (char*)0x16000000;
static int* fr = (int*)0x16000018;

void serial_putc(char c) {
    while (*fr & SERIAL_BUFFER_FULL);
    *dr = c;
}

void serial_puts(const char* str) {
    while (*str) {
        serial_putc(*str++);
    }
}
