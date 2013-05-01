.macro define_syscall name, id, args
.global \name
\name:
    mov r12, #\id
    swi #0
    bx lr
.endm

#include "../syscall.S"
