#pragma once

#include <arch/types.h>

typedef struct registers {
    uint32_t usr_r13, usr_r14;
    uint32_t cpsr, svc_r14;
    uint32_t r0, r1, r2, r3, r4, r5, r6;
    uint32_t r7, r8, r9, r10, r11, r12;
    uint32_t r15, spsr;
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

static inline void registers_set_stack(registers* regs, uintptr_t stack) {
    regs->usr_r13 = (uint32_t)stack;
}

static inline void irq_enable() {
    asm volatile("cpsie if");
}

static inline void irq_disable() {
    asm volatile("cpsid if");
}
