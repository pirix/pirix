#include <arch/i386.h>
#include <config/i386.h>

void timer_setup() {
    unsigned divisor = 1193180/TIMER_FREQ;

    outb(0x43, 0x36);

    unsigned char l = (unsigned char)(divisor & 0xff);
    unsigned char h = (unsigned char)((divisor >> 8) & 0xff);

    outb(0x40, l);
    outb(0x40, h);
}

void timer_clear() {

}
