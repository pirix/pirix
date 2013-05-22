#include <pirix/kprint.h>

#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
typedef __builtin_va_list va_list;

void kprintf(const char* fmt, ...) {
   va_list ap;
   va_start(ap, fmt);

   while (*fmt) {
       if (*fmt == '%') {
           fmt++;
           switch (*fmt) {
           case 'i':
               kputn(va_arg(ap, int), 10);
               break;

           case 'x':
           case 'p':
               kputs("0x");
               kputn(va_arg(ap, unsigned), 16);
               break;

           case 'b':
               kputs("0b");
               kputn(va_arg(ap, unsigned), 2);

           case 's':
               kputs(va_arg(ap, const char*));
               break;
           }
       }
       else {
           kputc(*fmt);
       }
       fmt++;
   }
   va_end(ap);
}

void kputc(char c) {
    extern void serial_putc(char);
    serial_putc(c);
}

void kputs(const char* str) {
    while (*str) {
        kputc(*str++);
    }
}

void kputn(unsigned long value, unsigned radix) {
    char buffer[65];
    char* ptr = buffer;
    const char* const chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned long temp;
    unsigned long digits;

    if (radix < 2 || radix > 36) return;

    temp = value;
    digits = 0;
    do {
        digits++;
        temp = temp/radix;
    } while (temp > 0);

    if (digits > 65) return;
    ptr += digits;
    *ptr = 0;

    temp = value;
    do {
        *--ptr = chars[temp%radix];
        temp = temp/radix;
    } while (--digits);

    kputs(ptr);
}
