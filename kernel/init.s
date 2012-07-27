.arm
.extern main
.extern irq_handle
.extern syscall

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

swi_handler:
    ldr r4, [r14, #-4]
    and r4, #0xffffff
    push {r4}
    bl syscall
    pop {r14}
    movs r15, r14

irq_handler:
    sub r14, #4

    /* backup registers */
    push {r0 - r12, r14}

    msr cpsr_ctl, #0xd3
    mrs r1, spsr
    mov r2, r13
    mov r3, r14
    msr cpsr_ctl, #0xd2
    mrs r0, spsr
    push {r0 - r3}

    stmdb r13, {r13, r14}^
    sub r13, #8

    /* call interrupt handler */
    mov r0, r13
    bl irq_handle
    mov r13, r0

    ldmia r13, {r13, r14}^
    add r13, #8

    pop {r0 - r3}
    msr spsr_all, r0
    msr cpsr_ctl, #0xd3
    msr spsr_all, r1
    mov r13, r2
    mov r14, r3
    msr cpsr_all, #0xd2

    /* restore registers */
    pop {r0 - r12, r14}

    /* jump back */
    movs r15, r14

.global irq_enable
irq_enable:
    mrs r0, cpsr
    bic r0, #0x80
    msr cpsr_ctl, r0
    bx lr

.global irq_disable
irq_disable:
    mrs r0, cpsr
    orr r0, #0x80
    msr cpsr_ctl, r0
    bx lr

.global paging_enable
paging_enable:
    /* set all domains to client modus */
    mov r0, #0x55
    orr r0, r0, r0, lsl #8
    orr r0, r0, r0, lsl #16
    mcr p15, 0, r0, c3, c0

    /* deactive cache */
    mrc p15, 0, r0, c1, c0
    bic r0, #0x0004
    bic r0, #0x1000
    mcr p15, 0, r0, c1, c0

    /* activate paging */
    mrc p15, 0, r0, c1, c0
    orr r0, #0x00000001
    bic r0, #0x00800000
    mcr p15, 0, r0, c1, c0
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 4
    bx lr

.global paging_invalidate_tlb
paging_invalidate_tlb:
    mov r0, #0
    mcr p15, 0, r0, c8, c7, 0
    bx lr

.global paging_invalidate_tlb_entry
paging_invalidate_tlb_entry:
    mcr p15, 0, r0, c8, c7, 1
    bx lr

.global paging_activate_transition_table
paging_activate_transition_table:
    mcr p15, 0, r0, c2, c0, 0
    bx lr

/* kernel stack */
kernel_stack_addr:
    .4byte kernel_stack

.section .bss
    .space 8192
kernel_stack:
