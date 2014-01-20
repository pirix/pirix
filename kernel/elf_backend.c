#include <elf.h>
#include <pirix/paging.h>
#include <pirix/memarea.h>

static void elf_open(memarea* area) {
    kprintf("elf_open\n");
    Elf32_Phdr* phdr = (Elf32_Phdr*)area->data.elf_segment;
    area->start = phdr->p_vaddr;
    area->end = phdr->p_vaddr + phdr->p_memsz;
}

static void elf_close(memarea* area) {
    kprintf("elf_close\n");
}

static pf_status elf_pagefault(memarea* area, uintptr_t addr, pf_type fault) {
    kprintf("elf_pagefault\n");
    Elf32_Phdr* phdr = (Elf32_Phdr*)area->data.elf_segment;
    paging_map(addr & 0xfffff000, area->data.elf_addr & 0xfffff000, PAGE_PERM_USER);
    return PF_RESOLVED;
}

const membackend elf_backend = {
    .name = "ELF",
    .open = elf_open,
    .close = elf_close,
    .pagefault = elf_pagefault
};
