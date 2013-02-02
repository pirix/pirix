#include <arch/i386.h>

#define PORT 0x3f8

#define WIDTH 80
#define HEIGHT 25

static char* vmem = (char*)0xb8000;
static char vattr = 0x0f;
static int vx = 0;
static int vy = 0;

void video_put(char c) {
    switch(c) {
    case '\n':
        vx = 0;
        vy++;
        break;

    case '\b':
        vx--;
        video_put(' ');
        vx--;
        break;

    default:
        vmem[(vx+(vy*WIDTH))*2] = c;
        vmem[(vx+(vy*WIDTH))*2+1] = vattr;
        vx++;
    }

    // check width
    if (vx >= WIDTH) {
        vx = 0;
        vy++;
    }

    // check height
    if (vy == HEIGHT) {
        video_scroll();
    }
    video_cursor();
}

void video_clear() {
    int i;
    for (i = 0; i < WIDTH*HEIGHT*2; i += 2) {
        vmem[i] = ' ';
        vmem[i+1] = vattr;
    }
    vx = 0;
    vy = 0;
    video_cursor();
}

void video_cursor() {
    unsigned int pos;
    pos = vy * WIDTH + vx;
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

void video_scroll() {
    int i;
    for (i = 0; i < WIDTH*(HEIGHT-1)*2; i += 2) {
        vmem[i] = vmem[i+WIDTH*2];
        vmem[i+1] = vmem[i+1+WIDTH*2];
    }
    for (i = WIDTH*(HEIGHT-1)*2; i < WIDTH*HEIGHT*2; i += 2) {
        vmem[i] = ' ';
        vmem[i+1] = vattr;
    }
    vy--;
}

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
    video_put(c);
}

char serial_getc() {
    while ((inb(PORT+5) & 1) == 0);
    return inb(PORT);
}
