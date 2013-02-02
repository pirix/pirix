#pragma once

#define PAGE_PERM_USER 1
#define PAGE_PERM_KRNL 2

typedef unsigned* paging_context;

void paging_init();
paging_context paging_create_context();
int paging_map(paging_context context, unsigned virt, unsigned phys, unsigned access);
unsigned paging_map_kernel(unsigned phys);
void paging_activate_context(paging_context context);
