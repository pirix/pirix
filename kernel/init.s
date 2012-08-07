.arm
.extern main
.extern panic
.extern irq_handle
.extern syscall

.section .init
loader:
    /* deactive llcache */
    mrc p15, 0, r0, c1, c0
    bic r0, #0x1000
    bic r0, #0x0004
    mcr p15, 0, r0, c1, c0

    /* invalidate caches and tld */
    mov r0, #0
    mcr p15, 0, r2, c7, c7
    mcr p15, 0, r2, c8, c7

    /* set domains to client modus */
    mov r0, #0x55
    orr r0, r0, r0, lsl #8
    orr r0, r0, r0, lsl #1
    mcr p15, 0, r0, c3, c0

    /* set translation table base control register */
    mrc p15, 0, r0, c2, c0, 2
    mov r0, #0x1
    mcr p15, 0, r0, c2, c0, 2

    /* set ttbs */
    ldr r0, [pc, #tt0_addr - 8 - .]
    mcr p15, 0, r0, c2, c0, 0
    ldr r0, [pc, #tt1_addr - 8 - .]
    mcr p15, 0, r0, c2, c0, 1

    /* enable paging */
    mrc p15, 0, r0, c1, c0
    orr r0, #0x1
    mcr p15, 0, r0, c1, c0

    /* jump to start */
    ldr r0, [pc, #start_addr - 8 - .]
    bx r0

start_addr:
    .4byte start

tt0_addr:
    .4byte tt0

tt1_addr:
    .4byte tt1

.balign 0x4000, 0
tt0:
    /* identity mapping for the first 2 GB */
    .equ addr, 0
    .rept 2048
      .4byte addr | 0x2
      .equ addr, addr + 0x00100000
    .endr

.balign 0x4000, 0
tt1:
    /* map 4MB starting at 0x0 to 0xC0000000 */
    .space 0x3000
    .4byte 0x00000002
    .4byte 0x00100002
    .4byte 0x00200002
    .4byte 0x00300002
    .space 0x0ff0

.section .text
.balign 0x10
vectors:
    b start
    b exc_handler
    b swi_handler
    b exc_handler
    b exc_handler
    b .
    b irq_handler
    b .

start:
    /* set exception vectors base addr */
    ldr r0, =vectors
    mcr p15, 0, r0, c12, c0, 0

    /* initialize exception stack */
    ldr r0, =exc_stack
    msr cpsr_ctl, #0xd7 /* abort mode */
    mov sp, r0
    msr cpsr_ctl, #0xdb /* undefined mode */
    mov sp, r0

    /* irq stack */
    msr cpsr_ctl, #0xd2
    ldr sp, =irq_stack

    /* supervisor mode */
    msr cpsr_ctl, #0xd3
    ldr sp, =kernel_stack

    /* fill .bss with zeros */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__
    mov r3, #0
1:
    cmp r1, r2
    stmltia r1!, {r3}
    blt 1b

    /* call the kernel main routine*/
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

exc_handler:
    bl panic
    b .

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

.section .bss
    .space 8192
exc_stack:
    .space 8192
irq_stack:
    .space 8192
kernel_stack:
