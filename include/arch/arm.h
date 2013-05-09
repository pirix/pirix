#pragma once

#define PAGE_SIZE 4096

typedef struct registers {
    unsigned usr_r13, usr_r14;
    unsigned cpsr, svc_r14;
    unsigned r0, r1, r2, r3, r4, r5, r6;
    unsigned r7, r8, r9, r10, r11, r12;
    unsigned r15, spsr;
} __attribute__((packed)) registers;

static inline void registers_init(registers* regs, void* entry) {
    *regs = (registers) {
        .spsr = 0x50, // user mode
        .r15 = (unsigned)entry
    };
}

static inline void registers_set_kernel_mode(registers* regs) {
    regs->spsr = 0x5F;
}

static inline void registers_set_stack(registers* regs, unsigned* stack) {
    regs->usr_r13 = (unsigned)stack;
}

static inline void irq_enable() {
    asm volatile("cpsie if");
}

static inline void irq_disable() {
    asm volatile("cpsid if");
}
