#pragma once

#include <pirix/types.h>
#include <arch/paging.h>

void paging_init();
pagedir paging_create_pagedir();
int paging_map(pagedir dir, uintptr_t virt, uintptr_t phys, int access);
uintptr_t paging_map_kernel(uintptr_t phys, size_t size);
void paging_unmap_kernel(uintptr_t virt, size_t size);
void paging_activate_pagedir(pagedir dir);
uintptr_t paging_getphys(uintptr_t);
