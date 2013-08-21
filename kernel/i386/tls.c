#include <pirix/thread.h>
#include <pirix/scheduler.h>
#include <pirix/string.h>
#include <i386/syscall.h>
#include <i386/tls.h>

void tls_activate(unsigned desc[2]) {
    extern unsigned gdt_tls[2];
    gdt_tls[0] = desc[0];
    gdt_tls[1] = desc[1];
}

int tls_set(struct tls_desc* desc) {
    thread* t = scheduler_current_thread();

    t->tls[0] = desc->limit & 0xffff | desc->base << 16;

    t->tls[1] = (desc->base >> 16) & 0xff
                | 0b11110010 << 8 | 0b11 << 22
                | desc->limit & 0xf0000
                | desc->base & 0xff000000;

    tls_activate(t->tls);

    return 0x33;
}
