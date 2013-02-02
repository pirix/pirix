#include "pirix/syscall.h"

.extern main
.global _start

_start:
    call main
    movl %eax, %ebx
    movl $SYS_EXIT, %eax
    int $0x80
.loop:
    hlt
    jmp .loop
