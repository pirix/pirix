#pragma once

#define PAGE_SIZE 4096

typedef struct registers {
    unsigned ds;
    unsigned edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned irq, err;
    unsigned eip, cs, eflags, usr_esp, ss;
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

static inline void invlpg(unsigned long addr) {
    asm volatile("invlpg (%0)" :: "r" (addr) : "memory");
}
