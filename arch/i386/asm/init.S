.extern main
.extern setup_gdt, setup_idt

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

    // save multiboot args
    movl %eax, mb_magic
    movl %ebx, mb_info

    // setup kernel stack
    movl $kernel_stack, %esp

    // initialize gdt and idt
    call setup_gdt
    call setup_idt

    // enable SSE
    movl %cr0, %eax
    and $0xfffb, %ax
    or $0x2, %ax
    mov %eax, %cr0
    mov %cr4, %eax
    or $(3 << 9), %ax
    mov %eax, %cr4

    // jump to higher half
    ljmp $0x08, $init

.align 0x4
multiboot:
    .long 0x1BADB002
    .long 0x3 # Flags: Module Page Align, Memory Information
    .long -(0x1BADB002 + 0x3)

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
    movl $0, %gs:0x30
    call main

1:
    hlt
    jmp 1b

.global idle
idle:
    hlt
    jmp idle

.section .bss

.align 4
.global mb_magic
mb_magic:
    .long 0

.align 4
.global mb_info
mb_info:
    .long 0

.align 4
kernel_stack_end:
    .space 0x4000
kernel_stack:

.align 0x1000
.global kernel_page_tables
kernel_page_tables:
    .space 0x100000
