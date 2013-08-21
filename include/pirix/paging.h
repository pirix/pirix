#pragma once

#include <pirix/types.h>
#include <arch/paging.h>

void paging_init();
paging_context paging_create_context();
int paging_map(paging_context context, uintptr_t virt, uintptr_t phys, int access);
uintptr_t paging_map_kernel(uintptr_t phys);
void paging_unmap_kernel(uintptr_t virt);
void paging_activate_context(paging_context context);
uintptr_t paging_getphys(uintptr_t);
