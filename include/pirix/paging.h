#pragma once

#define PAGE_PERM_USER 1
#define PAGE_PERM_KRNL 2

typedef unsigned* paging_context;

void paging_init();
paging_context paging_create_context();
int paging_map(paging_context context, unsigned long virt, unsigned long phys, unsigned access);
void* paging_map_kernel(unsigned long phys);
void paging_unmap_kernel(unsigned long virt);
void paging_activate_context(paging_context context);
void* paging_getphys(unsigned long virt);
