.arm
.extern irq_handle
.global irq_enable
.global irq_handler

irq_enable:
    mrs r0, cpsr
    bic r0, #0x80
    msr cpsr_ctl, r0
    bx lr

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
