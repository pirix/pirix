.arm
.extern syscall
.global swi_handler

swi_handler:
    ldr r4, [r14, #-4]
    and r4, #0xffffff
    push {r4}
    bl syscall
    pop {r14}
    movs r15, r14
