#include <pirix/paging.h>
#include <pirix/memory.h>
#include <string.h>

#define KERNEL_PT ((unsigned*)0x00100000)

static paging_context* kernel_context = NULL;

void paging_unmap_kernel(unsigned virt) {
    if ((virt >= 0x40000000) || (virt < 0x20000000)) {
        return;
    }
    KERNEL_PT[virt >> 12] = PTE_NP;
    paging_invalidate_tlb_entry(virt);
}

unsigned paging_map_kernel(unsigned phys) {
    if (phys < 0x20000000) {
        return phys;
    }

    for (unsigned virt = 0x20000000; virt < 0x30000000; virt += 0x1000) {
        if ((KERNEL_PT[virt >> 12] & PTE_TM) == PTE_NP) {
            KERNEL_PT[virt >> 12] = phys | PTE_SP | PTE_KRNL;
            return virt;
        }
    }

    return 0;
}

int paging_map(paging_context* context, unsigned virt, unsigned phys, unsigned access) {
    if (virt < 0x40000000) {
        KERNEL_PT[virt >> 12] = phys | PTE_SP | access;
        paging_invalidate_tlb_entry(virt);
        return 0;
    }

    unsigned tti = virt >> 20;
    unsigned pti = (virt >> 12) & 0xff;
    unsigned ppt;

    if ((context->ttable[tti] & TTE_TM) == TTE_NP) {
        ppt = memory_alloc();
        context->ttable[tti] = ppt | TTE_CP;
    }
    else {
        ppt = context->ttable[tti] & TTE_ADDRESS_MASK;
    }

    unsigned* pt = (unsigned*)paging_map_kernel(ppt);
    pt[pti] = phys | PTE_SP | access;
    paging_unmap_kernel((unsigned)pt);

    paging_invalidate_tlb_entry(virt);

    return 0;
}

paging_context* paging_create_context() {
    paging_context* context = (paging_context*)memory_alloc();

    context->ttable = (unsigned*)memory_alloc_aligned(4, 2);
    memset(context->ttable+1024, 0, 3072*WORD_SIZE);

    if (kernel_context) {
        memcpy(context->ttable, kernel_context->ttable, 1024*WORD_SIZE);
    }
    else {
        for (unsigned virt = 0; virt < 0x40000000; virt += 0x00100000) {
            context->ttable[virt >> 20] = ((unsigned)(KERNEL_PT+(virt >> 12)) | TTE_CP);
        }
    }

    return context;
}

void paging_activate_context(paging_context* context) {
    paging_activate_transition_table(context->ttable);
    paging_invalidate_tlb();
}

int paging_init() {
    kernel_context = paging_create_context();

    for (unsigned addr = 0; addr < 0x20000000; addr += 0x1000) {
        paging_map(kernel_context, addr, addr, PTE_KRNL);
    }

    paging_activate_context(kernel_context);
    paging_enable();

    return 0;
}
