#include "pirix/syscall.h"

.arm
.extern main
.global _start

_start:
    mov r0, #0
    mov r1, #0
    bl main
    swi #SYS_EXIT
    b .
