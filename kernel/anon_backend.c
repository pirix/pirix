#include <pirix/paging.h>
#include <pirix/frame.h>
#include <pirix/memarea.h>

static void anon_open(memarea* area) {
    kprintf("anon_open\n");
}

static void anon_close(memarea* area) {
    kprintf("anon_close\n");
}

static void anon_pagefault(memarea* area, uintptr_t addr) {
    kprintf("anon_pagefault\n");
    paging_map(addr & 0xfffff000, frame_alloc(), PAGE_PERM_USER);
}

const membackend anon_backend = {
    .name = "ANON",
    .open = anon_open,
    .close = anon_close,
    .pagefault = anon_pagefault
};
