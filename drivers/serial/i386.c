#include <arch/i386.h>

#define PORT 0x3f8

void serial_init() {
    outb(PORT+1, 0x00);
    outb(PORT+3, 0x80);
    outb(PORT+0, 0x03);
    outb(PORT+1, 0x00);
    outb(PORT+3, 0x03);
    outb(PORT+2, 0xC7);
    outb(PORT+4, 0x0B);
}

void serial_putc(char c) {
    while ((inb(PORT+5) & 0x20) == 0);
    outb(PORT, c);
}

char serial_getc() {
    while ((inb(PORT+5) & 1) == 0);
    return inb(PORT);
}
