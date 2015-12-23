.equ KERNEL_OFFSET, 0xfffffff800000000

.code32
.section .init

.extern __kernel_start
.extern __kernel_end
.global loader
loader:
    cli

    // save multiboot checksum and info
    movl $(_multiboot_magic - KERNEL_OFFSET), %edi
    movl %eax, 0(%edi)
    movl $(_multiboot_info - KERNEL_OFFSET), %edi
    movl %ebx, 0(%edi)

    // save kernel start and end address
    movl $(_kernel_start - KERNEL_OFFSET), %edi
    movl $__kernel_start, 0(%edi)
    movl $(_kernel_end - KERNEL_OFFSET), %edi
    movl $__kernel_end, 0(%edi)

    // setup the p4 table
    movl $p4, %edi
    movl %edi, %cr3

    // add p3 tables to p4 table
    movl $p4, %edi

    movl $p3.low, (%edi)
    orl $0b111, (%edi)

    movl $p3.high, 0xff8(%edi)
    orl $0b111, 0xff8(%edi)

    // identity map the first gigabyte
    movl $p3.low, %edi
    movl $0, (%edi)
    orl  $0b10000111, (%edi)

    // map the first gigabyte to 0xfffffff800000000
    movl $p3.high, %edi
    movl $0, 0xf00(%edi)
    orl  $0b10000111, 0xf00(%edi)

    // enable physical address extension
    mov %cr4, %eax
    orl $(1 << 5), %eax
    mov %eax, %cr4

    // set the long mode bit
    mov $0xC0000080, %ecx
    rdmsr
    orl $(1 << 8), %eax
    wrmsr

    // enable paging
    movl %cr0, %eax
    orl $(1 << 31), %eax
    movl %eax, %cr0

    // complete tss gdt entry
    movl $tss, %eax
    movw %ax, (gdt.tss+2)
    shr $16, %eax
    movb %al, (gdt.tss+4)
    movb %ah, (gdt.tss+7)

    // load the gdt
    lgdt (gdt.pointer)

    ljmp $0x08, $loader64

.align 0x4
multiboot_start:
    .long 0xe85250d6
    .long 0
    .long multiboot_end - multiboot_start
    .long 0x100000000 - (0xe85250d6 + 0 + (multiboot_end - multiboot_start))
    .long 0
    .long 8
multiboot_end:

.align 0x1000
p4:
    .space 8*512
p3.low:
    .space 8*512
p3.high:
    .space 8*512

.align 8
gdt:
gdt.null:
    .quad 0

gdt.code:
    .word 0xffff
    .word 0
    .byte 0
    .byte 0b10011010
    .byte 0b10101111
    .byte 0

gdt.data:
    .word 0xffff
    .word 0
    .byte 0
    .byte 0b10010010
    .byte 0b10001111
    .byte 0

gdt.tss:
    .word 0x0067
    .word 0
    .byte 0
    .byte 0b10001001
    .byte 0b10010000
    .byte 0

gdt.pointer:
    .word . - gdt - 1
    .quad gdt

tss:
    .space 8*65

.code64
loader64:
    movabs $init, %rax
    jmp *%rax

.extern main
.section .text
init:
  // clear the segment registers
  xor %ax, %ax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movw %ax, %ss

  movabs $kernel_stack_start, %rsp

  call main

  cli
  hlt

.section .bss
kernel_stack_end:
    .space 8*4096
kernel_stack_start:

.align 8
.global _kernel_start
_kernel_start:
    .quad 0

.global _kernel_end
_kernel_end:
    .quad 0

.global _multiboot_magic
_multiboot_magic:
    .quad 0

.global _multiboot_info
_multiboot_info:
    .quad 0
