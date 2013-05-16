.macro define_syscall name, id, args
.global \name
\name:
    push %ebp
    movl %esp, %ebp
    push %ebx
    push %edi
    .ifgt \args-3
    movl 20(%ebp), %edx
.endif
.ifgt \args-2
    movl 16(%ebp), %ecx
.endif
.ifgt \args-1
    movl 12(%ebp), %ebx
.endif
.ifgt \args-0
    movl 8(%ebp), %eax
.endif
    movl $(\id), %edi
    int $0x80
    pop %edi
    pop %ebx
    leave
    ret
.endm

#include "../syscall.S"
