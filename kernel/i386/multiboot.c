#include <pirix/kernel.h>
#include <pirix/kprint.h>
#include <pirix/string.h>
#include <pirix/paging.h>
#include <pirix/process.h>
#include <pirix/memory.h>
#include <pirix/boot.h>
#include <arch/i386/multiboot.h>
#include <elf.h>

static boot_info binfo;
static multiboot_info* mb_info;

boot_info* multiboot_init(unsigned long mb_magic, unsigned long mb_addr) {
    if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid multiboot magic");
    }

    mb_info = (multiboot_info*)mb_addr;

    strncpy(binfo.args, (char*)mb_info->cmdline, BOOT_ARGS_MAX);

    if (mb_info->mods_count < 1) {
        panic("no multiboot modules found");
    }

    return &binfo;
}

static void load_elf(Elf32_Ehdr* ehdr) {
    Elf32_Phdr* phdr = (Elf32_Phdr*)((unsigned)ehdr + ehdr->e_phoff);

    unsigned vaddr_min = 0;
    unsigned vaddr_max = 0;
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr->p_type == PT_LOAD) {
            if (!vaddr_min) {
                vaddr_min = phdr->p_vaddr;
            }
            else if (phdr->p_vaddr < vaddr_min) {
                vaddr_min = phdr->p_vaddr;
            }
            if (phdr->p_vaddr + phdr->p_memsz > vaddr_max) {
                vaddr_max = phdr->p_vaddr + phdr->p_memsz;
            }
        }
        phdr = (Elf32_Phdr*)((unsigned)phdr + ehdr->e_phentsize);
    }

    unsigned mem_start = vaddr_min & 0xfffff000;
    unsigned mem_end = (vaddr_max & 0xfffff000) + 0x1000;
    unsigned nframes = (mem_end - mem_start) / 0x1000;

    unsigned* frames[nframes];

    for (int i = 0; i < nframes; i++) {
        frames[i] = (unsigned*)memory_alloc();
    }

    paging_context context = paging_create_context();

    phdr = (Elf32_Phdr*)((unsigned)ehdr + ehdr->e_phoff);

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr->p_type != PT_LOAD) continue;

        unsigned start = (unsigned)ehdr + phdr->p_offset;
        unsigned pos = 0;

        while (pos < phdr->p_filesz) {
            unsigned copy_start = start + pos;
            unsigned copy_end = (copy_start + 0x1000) & 0xfffff000;
            unsigned copy_size = copy_end - copy_start;

            if (copy_size > phdr->p_filesz - pos) {
                copy_size = phdr->p_filesz - pos;
                copy_end = copy_start + copy_size;
            }

            unsigned frame = (phdr->p_vaddr+pos - vaddr_min)/0x1000;
            unsigned offset = copy_start & 0xfff;
            unsigned* dest = (unsigned*)((unsigned)(frames[frame] + offset));

            kprintf("start: %p end: %p size: %p frame: %i offset: %p dest: %p\n", copy_start, copy_end, copy_size, frame, offset, dest);
            memcpy(dest, (unsigned*)copy_start, copy_size);

            unsigned virt = phdr->p_vaddr + pos & 0xfffff000;
            unsigned phys = (unsigned)dest & 0xfffff000;
            paging_map(context, virt, phys, PAGE_PERM_USER);
            kprintf("map %p to %p\n", virt, phys);

            pos += copy_size;
        }

        while (pos < phdr->p_memsz) {
            unsigned zero_size = ((start + pos + 0x1000) & 0xfffff000) - start;

            if (zero_size > phdr->p_memsz - pos) {
                zero_size = phdr->p_memsz - pos;
            }

            unsigned frame = (phdr->p_vaddr+pos - vaddr_min)/0x1000;
            unsigned offset = (start + pos) & 0xfff;
            unsigned* dest = (unsigned*)((unsigned)(frames[frame] + offset));

            kprintf("zero %p at %p frame: %i dest: %p \n", zero_size, pos, frame, dest);
            memset(dest, 0, zero_size);

            unsigned virt = phdr->p_vaddr + pos & 0xfffff000;
            unsigned phys = (unsigned)dest & 0xfffff000;
            paging_map(context, virt, phys, PAGE_PERM_USER);

            pos += zero_size;
        }

        phdr = (Elf32_Phdr*)((unsigned)phdr + ehdr->e_phentsize);
    }

    process_create((void*)ehdr->e_entry, context);
}

void modules_init() {
    multiboot_mod_list* mod = (multiboot_mod_list*)mb_info->mods_addr;

    unsigned count = mb_info->mods_count;
    while (count--) {
        /*
        for (unsigned i = 0; i < mod_size; i += 0x1000) {
            paging_map(context, 0x8000+i, mod->start+i, PAGE_PERM_USER);
        }
        */
        load_elf((Elf32_Ehdr*)mod->start);

        /*
        Elf32_Shdr* shdr = (Elf32_Shdr*)(mod->start + ehdr->e_shoff);

        for (int i = ehdr->e_shnum; i > 0; i--) {
            if (shdr->sh_type == SHT_NOBITS & !(shdr->sh_flags & SHF_TLS)) {
                unsigned bss_start = mod->start + shdr->sh_addr - 0x8000;
                memset((void*)bss_start, 0, shdr->sh_size);
            }

            shdr = (Elf32_Shdr*)((unsigned)shdr + ehdr->e_shentsize);
        }
        */

        //process_create((void*)ehdr->e_entry, context);

        mod++;
    }
}
