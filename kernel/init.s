.arm
.extern main
.extern panic
.extern irq_handler
.extern swi_handler

.section .init
    b start
    b .
    b swi_handler
    b .
    b .
    b .
    b irq_handler
    b .

.section .text
start:
    ldr r13, [r15, #kernel_stack_addr - 8 - .]
    mov r0, r13
    msr cpsr_ctl, #0xd2
    add r13, r0, #8192
    msr cpsr_ctl, #0xd3
    bl main
    b .

kernel_stack_addr:
    .4byte kernel_stack

    .space 8192
.section .bss
    .space 8192
kernel_stack:
