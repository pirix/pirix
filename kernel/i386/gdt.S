.section .text
.global setup_gdt
setup_gdt:
    lgdt gdt_ptr
    mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
    jmp $0x08, $Lret
Lret:
    ret

.section .data
.align 0x8
gdt:
1:
    .quad 0
2:
    .short 0xffff, 0
    .byte 0, 0x9a, 0xcf, 0
3:
    .short 0xffff, 0
    .byte 0, 0x92, 0xcf, 0
4:
    .short 0xffff, 0
    .byte 0, 0xfa, 0xcf, 0
5:
    .short 0xffff, 0
    .byte 0, 0xf2, 0xcf, 0

gdt_ptr:
    .short . - gdt - 1
    .long gdt
