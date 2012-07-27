OUTPUT_ARCH("arm")

ENTRY(main)

SECTIONS {
    .text 0x40000000 : AT(0x40000000) {
        *(.text)
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
}
