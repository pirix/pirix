#pragma once

struct tls_desc {
    unsigned long base;
    unsigned int  limit;
};

#ifdef __KERNEL__
void tls_activate(unsigned desc[2]);
#endif
