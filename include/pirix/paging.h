#pragma once

#define TTE_TYPE_MASK 0x003
#define TTE_TYPE_FREE 0x000
#define TTE_TYPE_PAGE 0x001
#define TTE_TYPE_SECT 0x002

#define TTE_PERM_MASK 0xc00
#define TTE_PERM_KRNL 0x400
#define TTE_PERM_USER 0xc00

#define TTE_ADDR_MASK 0xfffffc00

#define PTE_TYPE_MASK 0x003
#define PTE_TYPE_FREE 0x000
#define PTE_TYPE_EXTP 0x002

#define PTE_PERM_MASK 0x30
#define PTE_PERM_KRNL 0x10
#define PTE_PERM_USER 0x30

#define PTE_ADDR_MASK 0xfffff000

typedef unsigned paging_context;

int paging_init();
paging_context* paging_create_context();
int paging_map(paging_context* context, unsigned virt, unsigned phys, unsigned access);
unsigned paging_map_kernel(unsigned phys);

void paging_activate_context(paging_context* context);
void paging_set_transition_table(unsigned* ttable);
void paging_invalidate_tlb();
void paging_invalidate_tlb_entry(unsigned entry);
