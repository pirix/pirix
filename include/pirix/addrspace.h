#pragma once

#include <pirix/types.h>
#include <pirix/paging.h>

typedef struct memarea memarea;

typedef struct addrspace {
    pagedir pagedir;
    memarea* areas;

    struct {
        uintptr_t start;
        uintptr_t used;
        uintptr_t size;
    } heap;
} addrspace;

addrspace* addrspace_new();
void addrspace_add_area(addrspace* self, memarea* area);
uintptr_t addrspace_sbrk(addrspace* self, int incr);
void addrspace_pagefault(addrspace* self, uintptr_t addr, pf_type fault);
void addrspace_delete(addrspace* self);
