#pragma once

typedef unsigned paging_context;

int paging_init();
paging_context* paging_create_context();
int paging_map(paging_context* context, unsigned virt, unsigned phys, unsigned access);
void paging_activate_context(paging_context* context);
void paging_activate_transition_table(unsigned* ttable);
void paging_invalidate_tlb();
void paging_invalidate_tlb_entry(unsigned entry);
