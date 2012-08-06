#include <pirix/paging.h>
#include <pirix/memory.h>
#include <string.h>

static paging_context* kernel_context = NULL;

int paging_map(paging_context* context, unsigned virt, unsigned phys, unsigned access) {
    return 0;
}

paging_context* paging_create_context() {
    paging_context* context = (paging_context*)memory_alloc_aligned(4, 2);
    memset(context, 0, 4096*WORD_SIZE);
    return context;
}

void paging_activate_context(paging_context* context) {
    paging_activate_transition_table(context);
    paging_invalidate_tlb();
}

int paging_init() {
    return 0;
}
