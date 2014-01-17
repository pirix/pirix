#include <pirix/syscall.h>

int main(int argc, char* argv[]) {
    int res;
    const char* hello = "Hello World";
    asm volatile("int $0x80" : "=a"(res) : "a"(hello), "D"(SYS_LOG) : "memory", "cc");
    while (1);
    return 0;
}
