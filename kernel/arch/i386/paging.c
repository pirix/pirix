#include <arch/i386/config.h>
#include <arch/i386/cpu.h>
#include <pirix/kprint.h>
#include <pirix/paging.h>
#include <pirix/frame.h>
#include <pirix/irq.h>
#include <pirix/process.h>
#include <pirix/kernel.h>
#include <pirix/string.h>

uint32_t* page_dir = (uint32_t*)0xffbff000;
uint32_t* page_tables = (uint32_t*)0xffc00000;

static pagedir current_pagedir = NULL;

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
     * frame_alloc should return a free memory address from this
     * block, otherwise we are in serious trouble.
     */

    // reuse boot directory
    extern uint32_t boot_page_dir;
    uint32_t* kpagedir = &boot_page_dir;

    // create page tables from 0xc0000000
    for (int i = 0; i < 255; i++) {
        uint32_t* table = (uint32_t*)frame_alloc();
        kpagedir[i+768] = (uintptr_t)table | 0x3;

        memset(table, 0, PAGE_SIZE);

        if (i == 0) {
            // map 0xc0000000 to the first 4MB
            for (int j = 0; j < 1024; j++) {
                table[j] = j*PAGE_SIZE | 0x103;
            }
        }
        else if (i == 254) {
            table[1023] = (uintptr_t)kpagedir | 0x3;
        }
    }

    kpagedir[1023] = (uintptr_t)kpagedir | 0x3;

    paging_activate_pagedir(kpagedir);
}

pagedir paging_create_pagedir() {
    uintptr_t pdir = frame_alloc();
    uint32_t* dir = (uint32_t*)paging_map_kernel(pdir, PAGE_SIZE);

    uintptr_t ptable = frame_alloc();
    uint32_t* table = (uint32_t*)paging_map_kernel(ptable, PAGE_SIZE);

    memset(table, 0, PAGE_SIZE);
    memset(dir, 0, 0xc00);
    memcpy(dir+768, page_dir+768, 0x3f8);

    table[1023] = pdir | 0x3;
    dir[1022] = ptable | 0x3;
    dir[1023] = pdir | 0x3;

    paging_unmap_kernel((uintptr_t)table, PAGE_SIZE);
    paging_unmap_kernel((uintptr_t)dir, PAGE_SIZE);

    return (pagedir)pdir;
}

int paging_map(pagedir pdir, uintptr_t virt, uintptr_t phys, int access) {
    uint32_t* dir, *table;
    int pdidx = virt >> 22;
    int ptidx = (virt >> 12) & 0x3ff;

    if (!pdir) pdir = current_pagedir;

    if (pdir == current_pagedir && page_dir[pdidx]) {
        page_tables[ptidx+0x400*pdidx] = phys | access | 0x3;
        invlpg(virt);
        return 0;
    }

    dir = (uint32_t*)paging_map_kernel((uintptr_t)pdir, PAGE_SIZE);

    // create page table if unexistent
    if (!(dir[pdidx] & 0x1)) {
        uintptr_t ptable = frame_alloc();
        table = (uint32_t*)paging_map_kernel(ptable, PAGE_SIZE);
        memset(table, 0, PAGE_SIZE);
        dir[pdidx] = ptable | access | 0x3;
    }
    else {
        table = (uint32_t*)paging_map_kernel(dir[pdidx] & 0xfffff000, PAGE_SIZE);
    }

    table[ptidx] = phys | access | 0x3;
    paging_unmap_kernel((uintptr_t)table, PAGE_SIZE);
    paging_unmap_kernel((uintptr_t)dir, PAGE_SIZE);

    invlpg(virt);

    return 0;
}

static int calculate_page_count(uintptr_t addr, size_t size) {
    int page_count = (size+PAGE_SIZE-1) / PAGE_SIZE;
    if ((addr % PAGE_SIZE) + size > PAGE_SIZE) {
        page_count++;
    }
    return page_count;
}

uintptr_t paging_map_kernel(uintptr_t phys, size_t size) {
    int page_count = calculate_page_count(phys, size);

    for (uintptr_t virt = 0xd0000000; virt < 0xe0000000; virt += PAGE_SIZE) {
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

void paging_unmap_kernel(uintptr_t virt, size_t size) {
    int page_count = calculate_page_count(virt, size);

    for (int i = 0; i < page_count; i++) {
        page_tables[virt >> 12] = 0;
        invlpg(virt);
        virt += PAGE_SIZE;
    }
}

void paging_activate_pagedir(pagedir dir) {
    if (dir == current_pagedir) return;
    asm volatile("mov %0, %%cr3" :: "r"(dir) : "memory");
    current_pagedir = dir;
}

uintptr_t paging_getphys(uintptr_t virt) {
    int pdidx = virt >> 22;
    int ptidx = (virt >> 12) & (0x3FF + 0x400*pdidx);

    if (!(page_dir[pdidx] & 0x1)) return 0;
    if (!(page_tables[ptidx] & 0x1)) return 0;
    return (page_tables[ptidx] & ~0xFFF) + (virt & 0xFFF);
}
