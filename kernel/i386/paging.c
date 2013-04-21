#include <arch/i386.h>
#include <pirix/paging.h>
#include <pirix/memory.h>
#include <pirix/irq.h>
#include <pirix/kernel.h>
#include <string.h>

unsigned* page_dir = (unsigned*)0xffbff000;
unsigned* page_tables = (unsigned*)0xffc00000;

static paging_context current_context = NULL;

registers* paging_fault(registers* regs) {
    unsigned long addr;
    asm volatile("mov %%cr2, %0" : "=r"(addr));
    kprintf("page fault at %p\n", addr);
    panic(0);
    return regs;
}

void paging_init() {
    kprintf("init paging\n");
    irq_register(14, &paging_fault);

    unsigned* kcontext = (unsigned*)memory_alloc_aligned(4, 2);

    memset(kcontext, 0, 768*4);

    for (int i = 0; i < 255; i++) {
        unsigned* table = (unsigned*)memory_alloc_aligned(4, 2);
        kcontext[i+768] = (unsigned long)table | 0x3;

        memset(table, 0, 4*1024);

        if (i == 0) {
            for (int j = 0; j < 1024; j++) {
                table[j] = j*0x1000 | 0x103;
            }
        }
        else if (i == 254) {
            table[1023] = (unsigned long)kcontext | 0x103;
        }
    }

    kcontext[1023] = (unsigned long)kcontext | 0x3;

    paging_activate_context(kcontext);
}

paging_context paging_create_context() {
    panic("create context");
    unsigned* pcontext = memory_alloc_aligned(4, 2);
    unsigned* context = paging_map_kernel(pcontext);

    memset(context, 1, 0x1000);
    return context;

    paging_unmap_kernel(context);
    return pcontext;
}

int paging_map(paging_context context, unsigned long virt, unsigned long phys, unsigned access) {
    unsigned long pdidx = virt >> 22;
    unsigned long ptidx = virt >> 12 & 0x3ff;
    unsigned* dir, table;

    if (context == current_context) {
        dir = page_dir;
    }
    else {
        dir = paging_map_kernel(context);
    }

    if (!(dir[pdidx] & 0x1)) {
        panic("have to create paging table");
    }

    if (context == current_context) {
        table = page_tables[ptidx];
    }
    else {
        table = paging_map_kernel(dir[pdidx]);
    }

    if (context == current_context) {
        invlpg(virt);
    }
    else {
        paging_unmap_kernel(dir);
        paging_unmap_kernel(table);
    }

    return 0;
}

void* paging_map_kernel(unsigned long phys) {
    for (unsigned long virt = 0xd0000000; virt < 0xe0000000; virt += 0x1000) {
        unsigned* entry = &page_tables[virt >> 12];

        if (!(*entry & 0x1)) {
            *entry = phys | 0x103;
            invlpg(virt);
            return virt + (phys & 0xfff);
        }
    }

    return 0;
}

void paging_unmap_kernel(unsigned long virt) {
    page_tables[virt >> 12] = 0;
    invlpg(virt);
}

void paging_activate_context(paging_context context) {
    asm volatile("mov %0, %%cr3" :: "r"(context));
    current_context = context;
}
