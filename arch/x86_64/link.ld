KERNEL_PHYS = 0x00100000;
KERNEL_VIRT = 0xfffffff800000000;

ENTRY(loader)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS {
    . = KERNEL_PHYS;
    __kernel_start = .;

    .init : ALIGN(0x1000) {
        *(.init*)
    }

    . += KERNEL_VIRT;

    .text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_VIRT) ALIGN(0x1000) {
        *(.text*)
    }

    .data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_VIRT) ALIGN(0x1000) {
        *(.data*)
    }

    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_VIRT) ALIGN(0x1000) {
        *(.rodata*)
    }

    .bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_VIRT) ALIGN(0x1000) {
        __bss_start__ = .;
        *(.bss*)
        __bss_end__ = .;
    }

    __kernel_end = . - KERNEL_VIRT;
}
