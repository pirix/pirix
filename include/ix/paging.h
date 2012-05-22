#pragma once

#define TTE_NP 0x000
#define TTE_CP 0x001
#define TTE_TM 0x003
#define TTE_ADDRESS_MASK 0xfffffc00

#define PTE_NP 0x000
#define PTE_SP 0x002
#define PTE_TM 0x003
#define PTE_KRNL 0x550
#define PTE_USER 0xff0
#define PTE_ADDRESS_MASK 0xfffff000

typedef struct paging_context {
    unsigned* ttable;
} paging_context;

void paging_init();
void paging_enable();
paging_context* paging_create_context();
void paging_activate_context(paging_context* context);
void paging_activate_transition_table(unsigned* ttable);
unsigned paging_map_kernel(unsigned phys);
void paging_invalidate_tlb();
void paging_invalidate_tlb_entry(unsigned entry);
