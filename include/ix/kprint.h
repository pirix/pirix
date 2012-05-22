#ifndef IX_KPRINT_H
#define IX_KPRINT_H

void kprintf(const char* fmt, ...);
void kputc(char c);
void kputs(const char* str);
void kputn(long value, unsigned radix);

#endif
