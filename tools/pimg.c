#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <libelf.h>
#include <pirix/boot.h>

static unsigned offset = 0x1000;
static boot_header* boothdr;
static Elf_Scn* boothdr_scn;

static char strtab[] = {
    '\0',
    '.', 't', 'e', 'x', 't', '\0',
    '.', 'd', 'a', 't', 'a', '\0',
    '.', 'r', 'o', 'd', 'a', 't', 'a', '\0',
    '.', 'b', 's', 's', '\0',
    '.', 's', 't', 'r', 't', 'a', 'b', '\0',
};

#define STR_TEXT 1
#define STR_DATA 7
#define STR_RODATA 13
#define STR_BSS 21
#define STR_STRTAB 26

void align_size(unsigned* size, unsigned align) {
    if (align == 0) return;

    *size = (*size + align - 1) & ~(align - 1);
}

Elf_Scn* elf_findscn(Elf* elf, const char* name) {
    Elf_Scn* scn = NULL;
    size_t shstrndx = 0;

    elf_getshdrstrndx(elf, &shstrndx);

    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        Elf32_Shdr* shdr = elf32_getshdr(scn);

        if (!shdr) continue;
        const char* secname = elf_strptr(elf, shstrndx, shdr->sh_name);
        if (secname && !strcmp(secname, name)) return scn;
    }

    return NULL;
}

unsigned image_appendscn(Elf* image, Elf_Scn* source, int name) {
    if (!source) return 0;
    Elf_Scn* target = elf_newscn(image);

    Elf_Data* tgt_data = elf_newdata(target);
    Elf_Data* src_data = NULL;
    src_data = elf_getdata(source, src_data);
    memcpy(tgt_data, src_data, sizeof(Elf_Data));

    Elf32_Shdr* src_shdr = elf32_getshdr(source);
    Elf32_Shdr* tgt_shdr = elf32_getshdr(target);
    memcpy(tgt_shdr, src_shdr, sizeof(Elf32_Shdr));

    tgt_shdr->sh_name = name;

    align_size(&offset, tgt_shdr->sh_addralign);
    tgt_shdr->sh_offset = offset;
    offset += tgt_shdr->sh_size;

    return tgt_shdr->sh_offset;
}

Elf_Scn* image_strtab(Elf* image) {
    Elf_Scn* strtab_scn = elf_newscn(image);

    Elf_Data* strtab_data = elf_newdata(strtab_scn);
    strtab_data->d_align = 1;
    strtab_data->d_buf = strtab;
    strtab_data->d_size = sizeof(strtab);
    strtab_data->d_type = ELF_T_BYTE;
    strtab_data->d_version = EV_CURRENT;

    Elf32_Shdr* strtab_shdr = elf32_getshdr(strtab_scn);
    strtab_shdr->sh_name = STR_STRTAB;
    strtab_shdr->sh_type = SHT_STRTAB;
    strtab_shdr->sh_size = strtab_data->d_size;
    strtab_shdr->sh_offset = offset;
    strtab_shdr->sh_addralign = 0x1;
    strtab_shdr->sh_entsize = 0;

    align_size(&offset, strtab_shdr->sh_addralign);
    strtab_shdr->sh_offset = offset;
    offset += strtab_shdr->sh_size;

    return strtab_scn;
}

void boothdr_create(Elf* image) {
    boothdr = malloc(sizeof(boot_header));
    memset(boothdr, 0, sizeof(boot_header));

    boothdr_scn = elf_newscn(image);

    Elf_Data* boothdr_data = elf_newdata(boothdr_scn);
    boothdr_data->d_align = 0x4;
    boothdr_data->d_buf = boothdr;
    boothdr_data->d_size = sizeof(boot_header);
    boothdr_data->d_type = ELF_T_BYTE;
    boothdr_data->d_version = EV_CURRENT;

    Elf32_Shdr* boothdr_shdr = elf32_getshdr(boothdr_scn);
    boothdr_shdr->sh_name = 13;
    boothdr_shdr->sh_type = SHT_PROGBITS;
    boothdr_shdr->sh_size = boothdr_data->d_size;
    boothdr_shdr->sh_offset = offset;
    boothdr_shdr->sh_addralign = 0x4;
    boothdr_shdr->sh_entsize = 0;
    boothdr_shdr->sh_flags = SHF_ALLOC;

    align_size(&offset, boothdr_shdr->sh_addralign);
    boothdr_shdr->sh_offset = offset;
    boothdr_shdr->sh_addr = offset;
    offset += boothdr_shdr->sh_size;

    boothdr->magic = BOOT_MAGIC;
}

void boothdr_add_module(const char* path, unsigned addr, unsigned size, unsigned entry) {
    if (boothdr->module_count + 1 > BOOT_MAX_MODULES) {
        fprintf(stderr, "too many modules\n");
    }

    const char* name = strrchr(path, '/');
    name = name ? name+1 : path;

    struct boot_module* mod = &boothdr->modules[boothdr->module_count++];
    strncpy(mod->name, name, BOOT_MAX_MODNAME);
    mod->addr = addr;
    mod->size = size;
    mod->entry = entry;
}

int main(int argc, char** argv) {
    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "elf library fail\n");
        return 1;
    }

    int kernel_fd = 0;
    const char* output_file = "output.img";

    // parse command line options
    while (1) {
        int option = getopt(argc, argv, "k:o:");
        if (option == -1) break;

        switch (option) {
        case 'k':
            kernel_fd = open(optarg, O_RDONLY);
            break;

        case 'o':
            output_file = optarg;
            break;
        }
    }

    // check if required options were given
    if (!kernel_fd) {
        fprintf(stderr, "error: no kernel specified\n");
        return 1;
    }

    printf("creating image...\n");

    int image_fd = open(output_file, O_WRONLY|O_CREAT, 0777);

    if (image_fd < 0) {
        fprintf(stderr, "error: could not open %s for writing\n", output_file);
        return 1;
    }

    Elf* image = elf_begin(image_fd, ELF_C_WRITE, NULL);
    Elf* kernel_elf = elf_begin(kernel_fd, ELF_C_READ, NULL);

    Elf32_Ehdr* kernel_ehdr = elf32_getehdr(kernel_elf);
    Elf32_Ehdr* ehdr = elf32_newehdr(image);

    memcpy(ehdr, kernel_ehdr, sizeof(Elf32_Ehdr));
    ehdr->e_type = ET_EXEC;
    ehdr->e_version = EV_CURRENT;
    ehdr->e_shstrndx = 0;
    ehdr->e_phoff = elf32_fsize(ELF_T_EHDR, 1, EV_CURRENT);
    ehdr->e_shentsize = elf32_fsize(ELF_T_SHDR, 1, EV_CURRENT);
    align_size(&ehdr->e_phoff, 4);

    elf_flagelf(image, ELF_C_SET, ELF_F_LAYOUT);

    image_appendscn(image, elf_findscn(kernel_elf, ".init"), STR_TEXT);
    image_appendscn(image, elf_findscn(kernel_elf, ".text"), STR_TEXT);
    image_appendscn(image, elf_findscn(kernel_elf, ".data"), STR_DATA);
    image_appendscn(image, elf_findscn(kernel_elf, ".rodata"), STR_RODATA);
    image_appendscn(image, elf_findscn(kernel_elf, ".bss"), STR_BSS);

    // create program header
    Elf32_Phdr* phdr = elf32_newphdr(image, 1);
    phdr->p_type = PT_LOAD;
    phdr->p_offset = elf32_getshdr(elf_getscn(image, 1))->sh_offset;
    phdr->p_paddr = ehdr->e_entry;
    phdr->p_vaddr = ehdr->e_entry;
    phdr->p_align = 0x8000;
    phdr->p_flags = PF_R | PF_W | PF_X;

    boothdr_create(image);

    for (unsigned module = 1; optind < argc; module++) {
        const char* mod = argv[optind++];
        int mod_fd = open(mod, O_RDONLY);

        if (!mod_fd) {
            fprintf(stderr, "could not open %s\n", mod);
            return 1;
        }

        align_size(&offset, 0x1000);

        Elf* mod_elf = elf_begin(mod_fd, ELF_C_READ, NULL);
        unsigned start = image_appendscn(image, elf_findscn(mod_elf, ".text"), STR_TEXT);
        image_appendscn(image, elf_findscn(mod_elf, ".data"), STR_DATA);
        image_appendscn(image, elf_findscn(mod_elf, ".rodata"), STR_RODATA);
        image_appendscn(image, elf_findscn(mod_elf, ".bss"), STR_BSS);

        Elf32_Ehdr* ehdr = elf32_getehdr(mod_elf);

        boothdr_add_module(mod, start, offset-start, ehdr->e_entry);
    }

    elf_update(image, ELF_C_NULL);

    unsigned start = elf32_getshdr(elf_getscn(image, 1))->sh_offset;
    unsigned end = elf32_getshdr(elf_getscn(image, ehdr->e_shnum-1))->sh_offset;
    unsigned filesz = end - start + elf32_getshdr(elf_getscn(image, ehdr->e_shnum-1))->sh_size;

    phdr->p_filesz = filesz;
    phdr->p_memsz = filesz;

    elf_update(image, ELF_C_NULL);

    // update section header offset
    align_size(&filesz, 4);
    ehdr->e_shoff = filesz;
    ehdr->e_shstrndx = elf_ndxscn(image_strtab(image));

    elf_update(image, ELF_C_WRITE);

    elf_end(image);

    close(image_fd);
    close(kernel_fd);

    return 0;
}
