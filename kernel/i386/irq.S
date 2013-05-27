.extern irq_handle, scheduler_schedule,  syscall, tss

.section .text
handle_irq:
    pusha

    pushl %ds

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp
    call irq_handle
    mov %eax, %esp

    // save %esp (which is also in %eax) to tss
    add $28, %eax
    movl %eax, (tss+4)

    popl %ebx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs

    popa

    // remove error codes
    add $8, %esp

    iret

handle_syscall:
    pushl %ds
    push %edi
    push %edx
    push %ecx
    push %ebx
    push %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call syscall

    // remove args from stack and save %esp to tss
    add $44, %esp
    mov %esp, (tss+4)
    sub $24, %esp

    pop %ebx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs

    iret

.global task_switch
task_switch:
    cli
    pushf
    pushl %cs
    push $task_switch_ret
    sub $8, %esp

    pusha

    pushl %ds

    push %esp
    call scheduler_schedule
    mov %eax, %esp

    // save %esp (which is also in %eax) to tss
    add $28, %eax
    movl %eax, (tss+4)

    pop %ebx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs

    popa

    // remove error codes
    add $8, %esp

    iret
task_switch_ret:
    ret

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

.macro set_gate nr isr flags
    movl $idt+\nr*8, %eax // idt offset
    movl $\isr, %ebx

    movw %bx, (%eax) // base 0-15
    movw $0x08, 2(%eax) // selector
    movb $0x00, 4(%eax) // zeros
    movb $\flags, 5(%eax) // flags
    shr $16, %ebx
    movw %bx, 6(%eax) // base 16-31
.endm

.global setup_idt
setup_idt:
    set_gate 0 irq_stub_0 0x8e
    set_gate 1 irq_stub_1 0x8e
    set_gate 2 irq_stub_2 0x8e
    set_gate 3 irq_stub_3 0x8e
    set_gate 4 irq_stub_4 0x8e
    set_gate 5 irq_stub_5 0x8e
    set_gate 6 irq_stub_6 0x8e
    set_gate 7 irq_stub_7 0x8e
    set_gate 8 irq_stub_8 0x8e
    set_gate 9 irq_stub_9 0x8e
    set_gate 10 irq_stub_10 0x8e
    set_gate 11 irq_stub_11 0x8e
    set_gate 12 irq_stub_12 0x8e
    set_gate 13 irq_stub_13 0x8e
    set_gate 14 irq_stub_14 0x8e
    set_gate 15 irq_stub_15 0x8e
    set_gate 16 irq_stub_16 0x8e
    set_gate 17 irq_stub_17 0x8e
    set_gate 32 irq_stub_32 0x8e
    set_gate 33 irq_stub_33 0x8e
    set_gate 34 irq_stub_34 0x8e
    set_gate 35 irq_stub_35 0x8e
    set_gate 36 irq_stub_36 0x8e
    set_gate 37 irq_stub_37 0x8e
    set_gate 38 irq_stub_38 0x8e
    set_gate 39 irq_stub_39 0x8e
    set_gate 40 irq_stub_40 0x8e
    set_gate 41 irq_stub_41 0x8e
    set_gate 42 irq_stub_42 0x8e
    set_gate 43 irq_stub_43 0x8e
    set_gate 44 irq_stub_44 0x8e
    set_gate 45 irq_stub_45 0x8e
    set_gate 46 irq_stub_46 0x8e
    set_gate 47 irq_stub_47 0x8e
    set_gate 128 handle_syscall 0xef
    lidt idt_ptr
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
