#include <arch/i386/multiboot.h>
#include <pirix/kernel.h>
#include <pirix/string.h>
#include <pirix/boot.h>

boot_info* multiboot_init(unsigned long mb_magic, multiboot_info* mb_info) {
    static boot_info binfo;

    if (mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid multiboot magic");
    }

    strncpy(binfo.args, (char*)mb_info->cmdline, BOOT_ARGS_MAX);

    if (mb_info->mod_count < 1) {
        panic("no multiboot modules found");
    }

    if (mb_info->mod_count > BOOT_MOD_MAX) {
        panic("too many multiboot modules");
    }

    binfo.mod_count = mb_info->mod_count;

    multiboot_mod_list* mod = (multiboot_mod_list*)mb_info->mod_addr;

    for (int i = 0; i < binfo.mod_count; i++) {
        binfo.modules[i].addr = (unsigned*)mod->start;
        binfo.modules[i].size = mod->end - mod->start;
        strncpy(binfo.modules[i].args, (char*)mod->args, BOOT_MOD_ARGS_MAX);
        mod++;
    }

    return &binfo;
}