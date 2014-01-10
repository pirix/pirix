#include <pirix/paging.h>
#include <pirix/frame.h>
#include <pirix/string.h>

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

#define PAGE_PERM_MASK 0x30

#define PTE_ADDR_MASK 0xfffff000

static paging_context kernel_context = NULL;

int paging_map(paging_context context, uintptr_t virt, uintptr_t phys, int access) {
    // translation table index
    int tti = virt >> 20;

    // page table index
    int pti = (virt >> 12) & 0xff;

    uintptr_t ppt;

    if ((context[tti] & TTE_TYPE_MASK) == TTE_TYPE_FREE) {
        ppt = frame_alloc();
        context[tti] = ppt | TTE_TYPE_PAGE;
    }
    else {
        ppt = context[tti] & TTE_ADDR_MASK;
    }

    uint32_t* pt = (uint32_t*)paging_map_kernel(ppt);
    pt[pti] = phys | PTE_TYPE_EXTP | access;

    // invalidate tlb entry
    asm volatile("mcr p15, 0, %0, c8, c7, 1" :: "r"(virt));

    return 0;
}

uintptr_t paging_map_kernel(uintptr_t phys) {
    if (phys > 0x80000000) return 0;
    return (void*)(0x80000000 + phys);
}

void paging_unmap_kernel(uintptr_t virt) {

}

paging_context paging_create_context() {
    paging_context context = (paging_context)frame_alloc_aligned(2, 2);
    paging_context kcontext = (paging_context)paging_map_kernel((uintptr_t)context);
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
    extern uintptr_t tt1;
    kernel_context = tt1;
}
