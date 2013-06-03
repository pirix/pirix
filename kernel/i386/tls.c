#include <pirix/thread.h>
#include <pirix/scheduler.h>
#include <pirix/string.h>
#include <arch/i386/syscall.h>
#include <arch/i386/tls.h>

void tls_activate(unsigned desc[2]) {
    extern unsigned gdt_tls[2];
    if (!desc[1]) return;
    gdt_tls[0] = desc[0];
    gdt_tls[1] = desc[1];
}

int tls_set(struct tls_desc* desc) {
    thread* t = scheduler_current_thread();

    t->tls[0] = desc->limit & 0xffff;
    t->tls[0] |= desc->base << 16;

    t->tls[1] = (desc->base >> 16) & 0xff;
    t->tls[1] |= 0b11110110 << 8;
    t->tls[1] |= desc->limit & 0xf0000;
    t->tls[1] |= 0b11 << 20;
    t->tls[1] |= desc->base & 0xff000000;

    tls_activate(t->tls);

    return 0x33;
}
