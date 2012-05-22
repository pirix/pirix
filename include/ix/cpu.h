#ifndef IX_CPU_H
#define IX_CPU_H

typedef struct cpu_state {
    unsigned usr_r13, usr_r14;
    unsigned cpsr, svc_spsr;
    unsigned svc_r13, svc_r14;
    unsigned r0, r1, r2, r3, r4, r5, r6;
    unsigned r7, r8, r9, r10, r11, r12, r15;
} __attribute__((packed)) cpu_state;

#endif
