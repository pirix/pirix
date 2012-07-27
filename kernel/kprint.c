#include <pirix/kprint.h>
#include <stdarg.h>

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
    serial_putc(c);
}

void kputs(const char* str) {
    serial_puts(str);
}

void kputn(long value, unsigned radix) {
    char buffer[65];
    char* ptr = buffer;
    const char* const chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned long temp;
    unsigned long digits;

    if (value < 0) {
        kputc('-');
        value = -value;
    }
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
