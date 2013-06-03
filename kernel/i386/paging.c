#include <arch/i386.h>
#include <pirix/kprint.h>
#include <pirix/paging.h>
#include <pirix/memory.h>
#include <pirix/irq.h>
#include <pirix/kernel.h>
#include <pirix/string.h>

unsigned* page_dir = (unsigned*)0xffbff000;
unsigned* page_tables = (unsigned*)0xffc00000;

static paging_context current_context = NULL;

registers* paging_fault(registers* regs) {
    unsigned long addr;
    asm volatile("mov %%cr2, %0" : "=r"(addr));
    kprintf("page fault at %p, eip: %p\n", addr, regs->eip);
    panic(0);
    return regs;
}

void paging_init() {
    irq_register(14, &paging_fault);

    /*
     * initialize kernel context
     *
     * addresses 0x0 - 0xc0000000 are currently identity mapped,
     * memory_alloc should return a free memory address from this
     * block, otherwise we are in serious trouble.
     */

    unsigned* kcontext = (unsigned*)memory_alloc();

    memset(kcontext, 0, 768*4);

    // create page tables from 0xc0000000
    for (int i = 0; i < 255; i++) {
        unsigned* table = (unsigned*)memory_alloc();
        kcontext[i+768] = (unsigned long)table | 0x3;

        memset(table, 0, 0x1000);

        if (i == 0) {
            // map 0xc0000000 to the first 4MB
            for (int j = 0; j < 1024; j++) {
                table[j] = j*0x1000 | 0x103;
            }
        }
        else if (i == 254) {
            table[1023] = (unsigned long)kcontext | 0x3;
        }
    }

    kcontext[1023] = (unsigned long)kcontext | 0x3;

    paging_activate_context(kcontext);
}

paging_context paging_create_context() {
    unsigned* pcontext = (unsigned*)memory_alloc();
    unsigned* context = paging_map_kernel((unsigned long)pcontext);

    unsigned* ptable = (unsigned*)memory_alloc();
    unsigned* table = paging_map_kernel((unsigned long)ptable);

    memset(table, 0, 0x1000);
    memset(context, 0, 0xc00);
    memcpy(context+768, page_dir+768, 0x3f8);

    table[1023] = (unsigned long)pcontext | 0x3;
    context[1022] = (unsigned long)ptable | 0x3;
    context[1023] = (unsigned long)pcontext | 0x3;

    paging_unmap_kernel((unsigned long)table);
    paging_unmap_kernel((unsigned long)context);

    return pcontext;
}

int paging_map(paging_context context, unsigned long virt, unsigned long phys, unsigned access) {
    unsigned* dir, *table;
    unsigned long pdidx = virt >> 22;
    unsigned long ptidx = (virt >> 12) & 0x3ff;

    access = (access == PAGE_PERM_USER) ? 0x4 : 0x0;

    if (!context) context = current_context;

    if (context == current_context && page_dir[pdidx]) {
        page_tables[ptidx+0x400*pdidx] = phys | access | 0x3;
        invlpg(virt);
        return 0;
    }

    dir = (unsigned*)paging_map_kernel((unsigned long)context);

    // create page table if unexistent
    if (!(dir[pdidx] & 0x1)) {
        unsigned* ptable = (unsigned*)memory_alloc();
        table = paging_map_kernel((unsigned long)ptable);
        memset(table, 0, 0x1000);
        dir[pdidx] = (unsigned long)ptable | access | 0x3;
    }
    else {
        table = paging_map_kernel(dir[pdidx] & 0xfffff000);
    }

    table[ptidx] = phys | access | 0x3;
    paging_unmap_kernel((unsigned long)table);
    paging_unmap_kernel((unsigned long)dir);

    invlpg(virt);

    return 0;
}

void* paging_map_kernel(unsigned long phys) {
    for (unsigned long virt = 0xd0000000; virt < 0xe0000000; virt += 0x1000) {
        unsigned* entry = &page_tables[virt >> 12];

        if (!(*entry & 0x1)) {
            *entry = phys | 0x103;
            invlpg(virt);
            return (void*)(virt + (phys & 0xfff));
        }
    }

    panic("kernel mapping area full");
    return 0;
}

void paging_unmap_kernel(unsigned long virt) {
    page_tables[virt >> 12] = 0;
    invlpg(virt);
}

void paging_activate_context(paging_context context) {
    if (context == current_context) return;
    asm volatile("mov %0, %%cr3" :: "r"(context) : "memory");
    current_context = context;
}

void* paging_getphys(unsigned long virt) {
    unsigned long pdidx = virt >> 22;
    unsigned long ptidx = (virt >> 12) & 0x3FF + 0x400*pdidx;

    if (!(page_dir[pdidx] & 0x1)) return 0;
    if (!(page_tables[ptidx] & 0x1)) return 0;
    return (void*)((page_tables[ptidx] & ~0xFFF) + (virt & 0xFFF));
}
