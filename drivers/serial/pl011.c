#define SERIAL_BUFFER_FULL (1 << 5)

static char* dr = (char*)0x96000000;
static int* fr = (int*)0x96000018;

void serial_init() {

}

void serial_putc(char c) {
    while (*fr & SERIAL_BUFFER_FULL);
    *dr = c;
}
