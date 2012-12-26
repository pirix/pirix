#include "pirix/syscall.h"

.arm
.extern main
.global _start

_start:
    bl main
    swi #SYS_EXIT
    b .
