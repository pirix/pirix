#include <arch/i386/multiboot.h>

.extern main, serial_init, multiboot_init
.extern setup_gdt, setup_idt
.extern multiboot_magic, multiboot_addr

.section .init
.global loader
loader:
    cli

    // set temporary page directory
    movl $boot_page_dir, %ecx
    movl %ecx, %cr3

    // enable PSE (4MB pages) and PGE (global pages)
    movl %cr4, %ecx
    or $0x00000090, %ecx
    movl %ecx, %cr4

    // enable paging
    movl %cr0, %ecx
    or $0x80000000, %ecx
    movl %ecx, %cr0

    // setup kernel stack
    movl $kernel_stack, %esp

    // push multiboot args
    push %ebx
    push %eax

    // initialize gdt and idt
    call setup_gdt
    call setup_idt

    // jump to higher half
    ljmp $0x08, $init

.align 0x4
multiboot:
    .long MULTIBOOT_HEADER_MAGIC
    .long 0x3 // Flags: Module Page Align, Memory Information
    .long -(MULTIBOOT_HEADER_MAGIC + 0x3)

/*
 * initial page table:
 * 0x00000000-0xc0000000 => 0x00000000-0xc0000000
 * 0xc0000000-0xffffffff => 0x00000000-0x3fffffff
 * flags 0x83: 4MB page, kernel mode
 */
.align 0x1000
.global boot_page_dir
boot_page_dir:
    .equ addr, 0
    .rept 768
        .long addr | 0x83
        .equ addr, addr + 0x400000
    .endr
    .equ addr, 0
    .rept 256
        .long addr | 0x83
        .equ addr, addr + 0x400000
    .endr

.section .text
init:
    call serial_init

    // returns boot_info structure in %eax
    call multiboot_init

    // remove multiboot args from stack
    add $8, %esp

    // pass boot_info to main
    push %eax
    call main

1:
    hlt
    jmp 1b

.global idle
idle:
    hlt
    jmp idle

.section .bss
    .space 0x1000
kernel_stack:
