#pragma once

#include <pirix/types.h>
#include <arch/paging.h>

typedef enum {
    PF_WRITE,
    PF_READ,
    PF_EXEC,
    PF_OTHER
} pf_type;

typedef enum {
    PF_RESOLVED,
    PF_FAULT
} pf_status;

void paging_init();
pagedir paging_create_pagedir();
void paging_map(uintptr_t virt, uintptr_t phys, int access);
uintptr_t paging_map_kernel(uintptr_t phys, size_t size);
void paging_unmap_kernel(uintptr_t virt, size_t size);
void paging_activate_pagedir(pagedir dir);
uintptr_t paging_getphys(uintptr_t);
