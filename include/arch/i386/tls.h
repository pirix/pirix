#pragma once

#include <arch/i386/types.h>

struct tls_desc {
    uintptr_t base;
    uintptr_t limit;
};

#ifdef __KERNEL__
void tls_activate(uintptr_t desc[2]);
#endif
