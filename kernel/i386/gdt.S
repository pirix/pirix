.section .text
.global setup_gdt
setup_gdt:
    // write tss address into gdt
    movl $gdt5, %eax
    movl $tss, %ebx
    movw %bx, 2(%eax) // base 0:15
    shr $16, %ebx
    movb %bl, 4(%eax) // base 16:23
    shr $8, %ebx
    movb %bl, 7(%eax) // base 24:31

    // load gdt and set segment registers
    lgdt gdt_ptr
    jmp $0x08, $Lret
Lret:
    mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss

    // load tss
    mov $(0x28 | 0x3), %ax
    ltr %ax
    ret

.section .data
.align 0x8
gdt0:
    .quad 0
gdt1:
    .short 0xffff, 0
    .byte 0, 0x9a, 0xcf, 0
gdt2:
    .short 0xffff, 0
    .byte 0, 0x92, 0xcf, 0
gdt3:
    .short 0xffff, 0
    .byte 0, 0xfa, 0xcf, 0
gdt4:
    .short 0xffff, 0
    .byte 0, 0xf2, 0xcf, 0
gdt5:
    .short 0x68, 0
    .byte 0, 0x89, 0x40, 0

gdt_ptr:
    .short . - gdt0 - 1
    .long gdt0

tss:
    .quad 0x0, 0x0, 0x10
    .space 29*4
