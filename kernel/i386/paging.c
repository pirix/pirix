#include <pirix/paging.h>
#include <pirix/memory.h>
#include <string.h>

static unsigned* kernel_context;

void paging_init() {
    extern unsigned* boot_page_dir;

    unsigned long ktables = memory_alloc_aligned(4*1024, 2);


    kprintf("ktables at %p\n", ktables);
}

paging_context paging_create_context() {
    //unsigned* kcontext = paging_map_kernel(context);
    memory_alloc_aligned(4, 2);

    //memset(kcontext, 0, 0x4000);
    //return kcontext;
    return 0;
}

int paging_map(paging_context context, unsigned long virt, unsigned long phys, unsigned access) {
    asm volatile("invlpg %0" :: "m"(virt));
    return 0;
}

void* paging_map_kernel(unsigned long phys) {
    return 0;
}

void paging_activate_context(paging_context context) {
    //asm volatile("mov %0, %%cr3" :: "r"(context));
}
