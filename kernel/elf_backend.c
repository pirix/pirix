#include <elf.h>
#include <pirix/paging.h>
#include <pirix/memarea.h>

static void elf_open(memarea* area) {
    Elf32_Phdr* phdr = (Elf32_Phdr*)area->data.elf_segment;
    area->start = phdr->p_vaddr;
    area->end = phdr->p_vaddr + phdr->p_memsz;
    kprintf("open elf %p %p-%p (%p)\n", phdr, area->start, area->end, area);
}

static void elf_close(memarea* area) {
    kprintf("elf_close\n");
}

static pf_status elf_pagefault(memarea* area, uintptr_t addr, pf_type fault) {
    Elf32_Phdr* phdr = (Elf32_Phdr*)area->data.elf_segment;

    uintptr_t segment_addr = area->data.elf_addr + phdr->p_offset;
    uintptr_t phys = segment_addr + (addr - phdr->p_vaddr);
    uintptr_t phys_fileend = segment_addr + phdr->p_filesz;
    uintptr_t phys_memend = segment_addr + phdr->p_memsz;

    uintptr_t virt_page = addr & 0xfffff000;
    uintptr_t phys_page = phys & 0xfffff000;

    if ((phys_page + 0x1000) > phys_fileend && phys_page < phys_memend) {
        uintptr_t data = paging_map_kernel(phys_page, 0x1000);
        paging_map(virt_page, frame_alloc(), PAGE_PERM_USER);
        memset(virt_page, 0, 0x1000);

        if (phys_memend - phys_page > 0x1000) {
            memcpy(virt_page, data, 0x1000);
        }
        else {
            memcpy(virt_page, data, phys_memend - phys_page);
        }

        paging_unmap_kernel(data, 0x1000);
    }
    else {
        paging_map(virt_page, phys_page, PAGE_PERM_USER);
    }


    return PF_RESOLVED;
}

const membackend elf_backend = {
    .name = "ELF",
    .open = elf_open,
    .close = elf_close,
    .pagefault = elf_pagefault
};
