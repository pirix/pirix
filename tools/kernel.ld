OUTPUT_ARCH("arm")

SECTIONS {
    .text 0x0008000 : AT(0x0008000) {
        *(.init)
        *(.text)
    }

    .data : ALIGN(4096) {
        *(.data)
    }

    .rodata : ALIGN(4096) {
        *(.rodata)
    }

    .bss : ALIGN(4096) {
        *(.bss)
    }

}
