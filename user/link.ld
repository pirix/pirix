OUTPUT_ARCH("arm")
OUTPUT_FORMAT("elf32-littlearm")

ENTRY(_start)

SECTIONS {
    . = 0x40000000;

    user_init_start = .;

    .text : {
        *(text)
    }

    .data ALIGN(4096) : {
        *(.data)
    }

    .rodata ALIGN(4096) : {
        *(.rodata)
    }

    .bss ALIGN(4096) : {
        *(.bss)
    }

    user_init_end = .;
}
