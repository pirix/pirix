ENTRY(_start)

SECTIONS {
    .text 0x8000 : AT(0x8000) {
        *(.init)
        *(.text*)
        *(.fini)
    }
    .data : {
        *(.data*)
        *(.ctors)
        *(.dtors)
        *(.jcr)
    }
    .rodata : {
        *(.rodata*)
        *(.eh_frame)
    }
    .bss : {
        *(.bss)
    }
}
