#pragma once

typedef struct registers {
    unsigned eax, ebx, ecx, edx, esi, edi, ebp;
    unsigned irq, err;
    unsigned eip, cs, eflags, esp, ss;
} __attribute__((packed)) registers;

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1" :: "a"(data), "Nd"(port));
}

static inline void irq_enable() {
    asm volatile("sti");
}

static inline void irq_disable() {
    asm volatile("cli");
}
