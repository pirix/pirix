#pragma once

typedef struct cpu_state {
    unsigned usr_r13, usr_r14;
    unsigned cpsr, svc_r14;
    unsigned r0, r1, r2, r3, r4, r5, r6;
    unsigned r7, r8, r9, r10, r11, r12;
    unsigned r15, spsr;
} __attribute__((packed)) cpu_state;