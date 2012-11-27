#include <config.h>

.arm
.extern main
.extern panic
.extern irq_handle
.extern syscall
.extern exception

.macro push_cpu_state
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

    mov r0, r13
.endm

.macro pop_cpu_state
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

    pop {r0 - r12, r14}
.endm

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
    ldr r0, =tt0
    mcr p15, 0, r0, c2, c0, 0
    ldr r0, =tt1
    mcr p15, 0, r0, c2, c0, 1

    /* enable paging */
    mrc p15, 0, r0, c1, c0
    orr r0, #0x1
    orr r0, #0x800000
    mcr p15, 0, r0, c1, c0

    /* jump to start */
    ldr r15, =start

.pool

.balign 0x4000, 0
.global tt0
tt0:
    /* identity mapping for the first 2 GB */
    .equ addr, 0
    .rept 2048
      .4byte addr | 0x402
      .equ addr, addr + 0x00100000
    .endr

.balign 0x4000, 0
.global tt1
tt1:
    .space 0x2000
    /* map the whole RAM to 0x80000000 */
    .equ addr, 0
    .rept 256
      .4byte addr | 0x402
      .equ addr, addr + 0x00100000
    .endr

/* map periphals to 0x90000000 */
#if PLATFORM == RBPI
    .equ addr, 0x20000000
    .rept 16
      .4byte addr | 0x402
      .equ addr, addr + 0x00100000
    .endr
    .space 0xbc0
#elif PLATFORM == QEMU
    .equ addr, 0x10000000
    .rept 256
      .4byte addr | 0x402
      .equ addr, addr + 0x00100000
    .endr
    .space 0x800
#else
    .space 0xfff
#endif

    /* map 1MB starting at 0x0 to 0xC0000000 */
    .4byte 0x00000402
    .space 0x0ffc

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
    stmfd sp!, {r4-r12,lr}

    ldr r4, [lr, #-4]
    and r4, #0xffffff
    push {r4}

    bl syscall

    pop {r4}
    ldmfd sp!, {r4-r12,pc}^

irq_handler:
    sub r14, #4

    push_cpu_state
    bl irq_handle
    pop_cpu_state

    /* jump back */
    movs r15, r14

exc_handler:
    push_cpu_state
    bl exception
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

.global paging_invalidate_tlb
paging_invalidate_tlb:
    mov r0, #0
    mcr p15, 0, r0, c8, c7, 0
    bx lr

.global paging_invalidate_tlb_entry
paging_invalidate_tlb_entry:
    mcr p15, 0, r0, c8, c7, 1
    bx lr

.global paging_set_transition_table
paging_set_transition_table:
    mcr p15, 0, r0, c2, c0, 0
    bx lr

.pool

.section .bss
    .space 8192
exc_stack:
    .space 8192
irq_stack:
    .space 8192
kernel_stack:
