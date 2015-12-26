.code64
.section .init

.extern irq_handler

.macro set_gate nr isr flags
set_gate_\nr:
    movq $idt + \nr * 16, %rax
    movabs $\isr, %rbx

    movw %bx, (%rax)      # set first 16 bit of base
    movw $0x08, 2(%rax)   # set code selector
    movw $\flags, 5(%rax) # set flags
    shr $16, %rbx
    movw %bx, 6(%rax)     # set middle 16 bit of base
    shr $16, %rbx
    movl %ebx, 8(%rax)    # set last 32 bit of base
.endm

.global init_irq
init_irq:
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

    lidt idt.pointer
    ret

.align 0x1000
idt:
    .space 256*16

.align 0x8
idt.pointer:
    .word 256 * 16 - 1
    .quad idt

.section .text
handle_irq:
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11

    movq %rsp, %rdi
    call irq_handle
    movq %rax, %rsp

    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax

    // remove error code and irq number from stack
    add $0x10, %rsp

    iretq

handle_syscall:
    cli
    hlt

.macro irq_stub nr
irq_stub_\nr:
    pushq $0
    pushq $\nr
    jmp handle_irq
.endm

.macro irq_stub_error nr
irq_stub_\nr:
    pushq $\nr
    jmp handle_irq
.endm

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
