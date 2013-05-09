#include <pirix/paging.h>
#include <pirix/memory.h>
#include <string.h>

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

static paging_context kernel_context = NULL;

int paging_map(paging_context context, unsigned long virt, unsigned long phys, unsigned access) {
    switch(access) {
        case PAGE_PERM_USER:
            access = PTE_PERM_USER;
            break;
        case PAGE_PERM_KRNL:
            access = PTE_PERM_KRNL;
            break;
        default:
            return -1;
    }

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

    // invalidate tlb entry
    asm volatile("mcr p15, 0, %0, c8, c7, 1" :: "r"(virt));

    return 0;
}

void* paging_map_kernel(unsigned long phys) {
    if (phys > 0x80000000) return 0;
    return (void*)(0x80000000 + phys);
}

paging_context paging_create_context() {
    paging_context context = (paging_context)memory_alloc_aligned(2, 2);
    paging_context kcontext = (paging_context)paging_map_kernel((unsigned)context);
    memset(kcontext, 0, 0x2000);
    return kcontext;
}

void paging_activate_context(paging_context context) {
    // set transition table
    asm volatile("mcr p15, 0, %0, c2, c0, 0" :: "r"(context));
    // invalidate tlb
    asm volatile("mcr p15, 0, %0, c8, c7, 0" :: "r"(0));
}

void paging_init() {
    extern unsigned* tt1;
    kernel_context = tt1;
}
