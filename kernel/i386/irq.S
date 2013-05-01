.extern irq_handle, syscall

.section .text
handle_irq:
    pusha

    mov %ds, %ax
    push %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp
    call irq_handle
    mov %eax, %esp

    pop %ebx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs

    popa

    add $8, %esp
    iret

handle_syscall:
    push %edi
    push %edx
    push %ecx
    push %ebx
    push %eax
    call syscall
    add $20, %esp
    iret

.macro irq_stub nr
.global irq_stub_\nr
irq_stub_\nr:
    pushl $0
    pushl $\nr
    jmp handle_irq
.endm

.macro irq_stub_error nr
.global irq_stub_\nr
irq_stub_\nr:
    pushl $\nr
    jmp handle_irq
.endm

.macro set_gate nr isr
    movl $idt+\nr*8, %eax // idt offset
    movl $\isr, %ebx

    movw %bx, (%eax) // base 0-15
    movw $0x08, 2(%eax) // selector
    movb $0x8e, 5(%eax) // flags
    shr $16, %ebx
    movw %bx, 6(%eax) // base 16-31
.endm

.global setup_idt
setup_idt:
    lidt idt_ptr
    set_gate 0 irq_stub_0
    set_gate 1 irq_stub_1
    set_gate 2 irq_stub_2
    set_gate 3 irq_stub_3
    set_gate 4 irq_stub_4
    set_gate 5 irq_stub_5
    set_gate 6 irq_stub_6
    set_gate 7 irq_stub_7
    set_gate 8 irq_stub_8
    set_gate 9 irq_stub_9
    set_gate 10 irq_stub_10
    set_gate 11 irq_stub_11
    set_gate 12 irq_stub_12
    set_gate 13 irq_stub_13
    set_gate 14 irq_stub_14
    set_gate 15 irq_stub_15
    set_gate 16 irq_stub_16
    set_gate 17 irq_stub_17
    set_gate 32 irq_stub_32
    set_gate 33 irq_stub_33
    set_gate 34 irq_stub_34
    set_gate 35 irq_stub_35
    set_gate 36 irq_stub_36
    set_gate 37 irq_stub_37
    set_gate 38 irq_stub_38
    set_gate 39 irq_stub_39
    set_gate 40 irq_stub_40
    set_gate 41 irq_stub_41
    set_gate 42 irq_stub_42
    set_gate 43 irq_stub_43
    set_gate 44 irq_stub_44
    set_gate 45 irq_stub_45
    set_gate 46 irq_stub_46
    set_gate 47 irq_stub_47
    set_gate 128 handle_syscall
    ret

irq_stub 0
irq_stub 1
irq_stub 2
irq_stub 3
irq_stub 4
irq_stub 5
irq_stub 6
irq_stub 7
irq_stub_error 8
irq_stub 9
irq_stub_error 10
irq_stub_error 11
irq_stub_error 12
irq_stub_error 13
irq_stub_error 14
irq_stub 15
irq_stub 16
irq_stub_error 17
irq_stub 32
irq_stub 33
irq_stub 34
irq_stub 35
irq_stub 36
irq_stub 37
irq_stub 38
irq_stub 39
irq_stub 40
irq_stub 41
irq_stub 42
irq_stub 43
irq_stub 44
irq_stub 45
irq_stub 46
irq_stub 47
irq_stub 128

.section .data
.align 0x8
idt:
    .fill 256, 8, 0

.global idt_ptr
idt_ptr:
    .short . - idt - 1
    .long idt
