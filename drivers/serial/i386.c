#include <arch/i386.h>
#include <pirix/string.h>

static short* vmem = (short*)0xc00b8000;
static int vx = 0, vy = 0;

void video_scroll() {
    memcpy(vmem, vmem+80, sizeof(short)*80*24);
    memset(vmem+(80*24), 0, sizeof(short)*80);
    vy--;
}

void video_cursor() {
    unsigned pos = vy * 80 + vx;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

void video_putc(char c) {
    switch(c) {
    case '\n':
        vx = 0;
        vy++;
        break;

    case '\b':
        vx--;
        video_putc(' ');
        vx--;
        break;

    default:
        vmem[vx+vy*80] = 0x0f << 8 | c;
        vx++;
    }

    if (vx >= 80) { vx = 0; vy++; }
    if (vy >= 25) video_scroll();
    video_cursor();
}

void video_clear() {
    memset(vmem, 0, sizeof(short)*80*25);
    vx = vy = 0;
    video_cursor();
}

#define PORT 0x3f8

void serial_init() {
    outb(PORT+1, 0x00);
    outb(PORT+3, 0x80);
    outb(PORT+0, 0x03);
    outb(PORT+1, 0x00);
    outb(PORT+3, 0x03);
    outb(PORT+2, 0xC7);
    outb(PORT+4, 0x0B);
    video_clear();
}

void serial_putc(char c) {
    while ((inb(PORT+5) & 0x20) == 0);
    outb(PORT, c);
    video_putc(c);
}

char serial_getc() {
    while ((inb(PORT+5) & 1) == 0);
    return inb(PORT);
}
