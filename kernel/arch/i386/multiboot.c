#include <arch/i386/multiboot.h>
#include <pirix/kernel.h>
#include <pirix/string.h>
#include <pirix/boot.h>

static boot_info binfo;

static void multiboot_modules(int count, multiboot_mod_list* mod) {
    if (count < 1) {
        panic("no multiboot modules found");
    }

    if (count > BOOT_MOD_MAX) {
        panic("too many multiboot modules");
    }

    for (int i = 0; i < count; i++) {
        binfo.modules[i].addr = (uintptr_t)mod->start;
        binfo.modules[i].size = mod->end - mod->start;
        strncpy(binfo.modules[i].args, (char*)mod->args, BOOT_MOD_ARGS_MAX);
        mod++;
    }

    binfo.mod_count = count;
}

static void multiboot_mmap(int length, multiboot_mmap_entry* mmap) {
    for (int i = 0; i < length; i++) {
        //! TODO
        mmap++;
    }
}

boot_info* multiboot_init(unsigned long mb_magic, multiboot_info* mb_info) {
    if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid multiboot magic");
    }

    strncpy(binfo.args, (char*)mb_info->cmdline, BOOT_ARGS_MAX);

    multiboot_modules(mb_info->mod_count, (void*)mb_info->mod_addr);
    multiboot_mmap(mb_info->mmap_length, (void*)mb_info->mmap_addr);

    return &binfo;
}
