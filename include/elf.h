#pragma once

#include <stdint.h>

#define EI_NIDENT   16
#define EI_CLASS    4
#define EI_DATA     5
#define EI_VERSION  6
#define ELFCLASS32  1
#define ELFDATA2LSB 1
#define ET_EXEC     2
#define EM_ARM      40
#define PT_LOAD     1

typedef uint32_t elf_addr;
typedef uint16_t elf_half;
typedef uint32_t elf_off;
typedef int32_t  elf_sword;
typedef uint32_t elf_word;

typedef struct elf_header {
    unsigned char ident[EI_NIDENT];
    elf_half type;
    elf_half machine;
    elf_word version;
    elf_addr entry;
    elf_off  phoff;
    elf_off  shoff;
    elf_word flags;
    elf_half ehsize;
    elf_half phentsize;
    elf_half phnum;
    elf_half shentsize;
    elf_half shnum;
    elf_half shstrndx;
} elf_header;

typedef struct elf_pheader {
    elf_word type;
    elf_off  offset;
    elf_addr vaddr;
    elf_addr paddr;
    elf_word filesz;
    elf_word memsz;
    elf_word flags;
    elf_word align;
} elf_pheader;
