#include <arch/arm/config.h>

.arm
.arch armv6
.extern main
.extern panic
.extern irq_handle
.extern scheduler_schedule
.extern syscall
.extern exception

#define USR_MODE 16
#define FIQ_MODE 17
#define IRQ_MODE 18
#define SVC_MODE 19
#define ABT_MODE 23
#define UND_MODE 27

.section .init
.global loader
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

/* map peripherals to 0x90000000 */
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

    /* initialize stacks */
    cps ABT_MODE
    ldr r0, =exc_stack
    cps UND_MODE
    ldr r0, =exc_stack
    cps IRQ_MODE
    ldr sp, =irq_stack
    cps #SVC_MODE
    ldr sp, =kernel_stack

    /* fill .bss with zeros */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__
    mov r3, #0
1:
    cmp r1, r2
    stmltia r1!, {r3}
    blt 1b

    /* call the kernel main routine */
    bl main
    b .

swi_handler:
    stmfd sp!, {r4-r11,lr}
    push {r12}
    bl syscall
    pop {r12}
    ldmfd sp!, {r4-r11,pc}^

irq_handler:
    sub lr, #4

    srsdb sp!, #SVC_MODE
    cps #SVC_MODE

    push {r0-r12}
    mrs r0, cpsr
    push {r0, r14}
    stmdb r13, {r13, r14}^
    sub r13, #8

    mov r0, r13
    bl irq_handle
    mov r13, r0

    ldmia r13, {r13, r14}^
    add r13, #8
    pop {r0, r14}
    msr cpsr, r0
    pop {r0-r12}

    rfeia r13!

exc_handler:
    srsdb sp!, #SVC_MODE
    cps #SVC_MODE

    push {r0-r12}
    mrs r0, cpsr
    push {r0, r14}
    stmdb r13, {r13, r14}^
    sub r13, #8

    mov r0, r13
    bl exception
    b .

.global arch_switch_task
arch_switch_task:
    ldr r0, =.ret
    mrs r1, cpsr
    push {r0, r1}

    push {r0-r12}
    mrs r0, cpsr
    push {r0, r14}
    stmdb r13, {r13, r14}^
    sub r13, #8

    mov r0, r13
    bl scheduler_schedule
    mov r13, r0

    ldmia r13, {r13, r14}^
    add r13, #8
    pop {r0, r14}
    msr cpsr, r0
    pop {r0-r12}

    rfeia r13!
.ret:
    bx lr

.global idle
idle:
    b .

.pool

.section .bss
    .space 8192
exc_stack:
    .space 8192
irq_stack:
    .space 8192
kernel_stack:
