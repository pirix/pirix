#define MB_MAGIC 0x1badb002
#define MB_FLAGS 0x3
#define MB_CHECK -(MB_MAGIC + MB_FLAGS)

.extern main
.extern setup_gdt, setup_idt

.section .init
.global loader
loader:
    /* set temporary stack */
    movl $boot_stack, %esp

    /* set temporary page directory */
    movl $boot_page_dir, %ecx
    movl %ecx, %cr3

    /* enable PSE (4MB pages) */
    movl %cr4, %ecx
    or $0x00000010, %ecx
    movl %ecx, %cr4

    /* enable paging */
    movl %cr0, %ecx
    or $0x80000000, %ecx
    movl %ecx, %cr0

    call setup_gdt
    call setup_idt

    /* jump to higher half */
    ljmp $0x08, $init

.align 0x4
multiboot:
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECK

.align 0x4
    .space 128
boot_stack:

.align 0x1000
.global boot_page_dir
boot_page_dir:
    .long 0x00000083
    .rept 767
        .long 0x0
    .endr
    .long 0x00000083
    .rept 255
        .long 0x0
    .endr

.section .text
init:
    movl $kernel_stack, %esp
    call main

1:
    hlt
    jmp 1b

.global task_switch
task_switch:
    ret

.global idle
idle:
    hlt
    jmp idle

.section .bss
    .space 8192
kernel_stack:
