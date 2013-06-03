.section .text
.global setup_gdt
setup_gdt:
    // write tss address into gdt
    movl $gdt_tss, %eax
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
.global gdt_tls
gdt:
    .quad 0
gdt_kernel_code:
    .short 0xffff, 0
    .byte 0, 0x9a, 0xcf, 0
gdt_kernel_data:
    .short 0xffff, 0
    .byte 0, 0x92, 0xcf, 0
gdt_user_code:
    .short 0xffff, 0
    .byte 0, 0xfa, 0xcf, 0
gdt_user_data:
    .short 0xffff, 0
    .byte 0, 0xf2, 0xcf, 0
gdt_tss:
    .short 0x68, 0
    .byte 0, 0x89, 0x40, 0
gdt_tls:
    .short 0, 0
    .byte 0, 0, 0, 0

gdt_ptr:
    .short . - gdt - 1
    .long gdt

.global tss
tss:
    .long 0x0
    .long 0x0, 0x10 // esp0, ss0
    .long 0x0, 0x0  // esp1, ss1
    .long 0x0, 0x0  // esp2, ss2
    .space 19*4
