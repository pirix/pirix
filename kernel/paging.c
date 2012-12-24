#include <pirix/paging.h>
#include <pirix/memory.h>
#include <string.h>

static paging_context* kernel_context = NULL;

int paging_map(paging_context* context, unsigned virt, unsigned phys, unsigned access) {
    // translation table index
    unsigned tti = virt >> 20;

    // page table index
    unsigned pti = (virt >> 12) & 0xff;

    unsigned ppt;

    if ((context[tti] & TTE_TYPE_MASK) == TTE_TYPE_FREE) {
        ppt = (unsigned)memory_alloc();
        context[tti] = ppt | TTE_TYPE_PAGE;
    }
    else {
        ppt = context[tti] & TTE_ADDR_MASK;
    }

    unsigned* pt = (unsigned*)paging_map_kernel(ppt);
    pt[pti] = phys | PTE_TYPE_EXTP | access;

    paging_invalidate_tlb_entry(virt);
    return 0;
}

unsigned paging_map_kernel(unsigned phys) {
    if (phys > 0x8000000) return 0;
    return 0x80000000 + phys;
}

paging_context* paging_create_context() {
    paging_context* context = (paging_context*)memory_alloc_aligned(2, 2);
    paging_context* kcontext = (paging_context*)paging_map_kernel((unsigned)context);
    memset(kcontext, 0, 2048*WORD_SIZE);
    return kcontext;
}

void paging_activate_context(paging_context* context) {
    paging_set_transition_table(context);
    paging_invalidate_tlb();
}

int paging_init() {
    extern unsigned* tt1;
    kernel_context = tt1;
    return 0;
}
