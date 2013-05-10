#include "pirix/syscall.h"

.extern main
.global _start

_start:
    call main
    movl $SYS_EXIT, %edi
    int $0x80
    jmp .
