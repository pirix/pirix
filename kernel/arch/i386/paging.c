#include <arch/i386/config.h>
#include <arch/i386/cpu.h>
#include <pirix/kprint.h>
#include <pirix/paging.h>
#include <pirix/memory.h>
#include <pirix/irq.h>
#include <pirix/process.h>
#include <pirix/kernel.h>
#include <pirix/string.h>

uint32_t* page_dir = (uint32_t*)0xffbff000;
uint32_t* page_tables = (uint32_t*)0xffc00000;

static paging_context current_context = NULL;

registers* paging_fault(registers* regs) {
    uintptr_t addr;
    asm volatile("mov %%cr2, %0" : "=r"(addr));
    kprintf("page fault at %p, eip: %p\n", addr, regs->eip);
    process_exit(128);
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

    // reuse boot directory
    extern uint32_t boot_page_dir;
    uint32_t* kcontext = &boot_page_dir;

    // create page tables from 0xc0000000
    for (int i = 0; i < 255; i++) {
        uint32_t* table = (uint32_t*)memory_alloc();
        kcontext[i+768] = (uintptr_t)table | 0x3;

        memset(table, 0, 0x1000);

        if (i == 0) {
            // map 0xc0000000 to the first 4MB
            for (int j = 0; j < 1024; j++) {
                table[j] = j*0x1000 | 0x103;
            }
        }
        else if (i == 254) {
            table[1023] = (uintptr_t)kcontext | 0x3;
        }
    }

    kcontext[1023] = (uintptr_t)kcontext | 0x3;

    paging_activate_context(kcontext);
}

paging_context paging_create_context() {
    uintptr_t pcontext = memory_alloc();
    uint32_t* context = (uint32_t*)paging_map_kernel(pcontext);

    uintptr_t ptable = memory_alloc();
    uint32_t* table = (uint32_t*)paging_map_kernel(ptable);

    memset(table, 0, 0x1000);
    memset(context, 0, 0xc00);
    memcpy(context+768, page_dir+768, 0x3f8);

    table[1023] = pcontext | 0x3;
    context[1022] = ptable | 0x3;
    context[1023] = pcontext | 0x3;

    paging_unmap_kernel((uintptr_t)table);
    paging_unmap_kernel((uintptr_t)context);

    return (paging_context)pcontext;
}

int paging_map(paging_context context, uintptr_t virt, uintptr_t phys, int access) {
    uint32_t* dir, *table;
    int pdidx = virt >> 22;
    int ptidx = (virt >> 12) & 0x3ff;

    if (!context) context = current_context;

    if (context == current_context && page_dir[pdidx]) {
        page_tables[ptidx+0x400*pdidx] = phys | access | 0x3;
        invlpg(virt);
        return 0;
    }

    dir = (unsigned*)paging_map_kernel((unsigned long)context);

    // create page table if unexistent
    if (!(dir[pdidx] & 0x1)) {
        uintptr_t ptable = memory_alloc();
        table = (uint32_t*)paging_map_kernel(ptable);
        memset(table, 0, 0x1000);
        dir[pdidx] = ptable | access | 0x3;
    }
    else {
        table = (uint32_t*)paging_map_kernel(dir[pdidx] & 0xfffff000);
    }

    table[ptidx] = phys | access | 0x3;
    paging_unmap_kernel((uintptr_t)table);
    paging_unmap_kernel((uintptr_t)dir);

    invlpg(virt);

    return 0;
}

uintptr_t paging_map_kernel(uintptr_t phys) {
    for (uintptr_t virt = 0xd0000000; virt < 0xe0000000; virt += 0x1000) {
        uint32_t* entry = &page_tables[virt >> 12];

        if (!(*entry & 0x1)) {
            *entry = phys | 0x103;
            invlpg(virt);
            return (virt + (phys & 0xfff));
        }
    }

    panic("kernel mapping area full");
    return 0;
}

void paging_unmap_kernel(uintptr_t virt) {
    page_tables[virt >> 12] = 0;
    invlpg(virt);
}

void paging_activate_context(paging_context context) {
    if (context == current_context) return;
    asm volatile("mov %0, %%cr3" :: "r"(context) : "memory");
    current_context = context;
}

uintptr_t paging_getphys(uintptr_t virt) {
    int pdidx = virt >> 22;
    int ptidx = (virt >> 12) & (0x3FF + 0x400*pdidx);

    if (!(page_dir[pdidx] & 0x1)) return 0;
    if (!(page_tables[ptidx] & 0x1)) return 0;
    return (page_tables[ptidx] & ~0xFFF) + (virt & 0xFFF);
}
