#pragma once

typedef struct registers {
    unsigned gs, ds;
    unsigned edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned irq, err;
    unsigned eip, cs, eflags, usr_esp, ss;
} __attribute__((packed)) registers;

static inline void registers_init(registers* regs, void* entry) {
    *regs = (registers) {
        .eip = (unsigned)entry,
        .cs = 0x18 | 0x3,
        .ss = 0x20 | 0x3,
        .ds = 0x20 | 0x3,
        .gs = 0x20 | 0x3,
        .eflags = 0x202,
    };
}

static inline void registers_set_kernel_mode(registers* regs) {
    regs->cs = 0x08 | 0x3;
    regs->ss = 0x10 | 0x3;
    regs->ds = 0x10 | 0x3;
    regs->gs = 0x10 | 0x3;
    regs->eflags = 0x202;
}

static inline void registers_set_stack(registers* regs, unsigned* stack) {
    regs->usr_esp = (unsigned)stack;
}

static inline void irq_enable() {
    asm volatile("sti");
}

static inline void irq_disable() {
    asm volatile("cli");
}

static inline void invlpg(unsigned long addr) {
    asm volatile("invlpg (%0)" :: "r"(addr) : "memory");
}
