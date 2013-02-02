.macro define_syscall name, id
.global \name
\name:
    movl $(\id), %eax
    int $0x80
    ret
.endm

#include "../syscall.S"
