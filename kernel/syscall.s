.arm
.extern syscall
.global swi_handler

swi_handler:
    b .
    push {r14}
    bl syscall
    pop {r14}
    movs r15, r14
