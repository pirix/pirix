.arm
.global paging_enable
.global paging_activate_transition_table
.global paging_invalidate_tlb
.global paging_invalidate_tlb_entry

paging_enable:
    /* set all domains to client modus */
    mov r0, #0x55
    orr r0, r0, r0, lsl #8
    orr r0, r0, r0, lsl #16
    mcr p15, 0, r0, c3, c0

    /* deactive cache */
    mrc p15, 0, r0, c1, c0
    bic r0, #0x0004
    bic r0, #0x1000
    mcr p15, 0, r0, c1, c0

    /* activate paging */
    mrc p15, 0, r0, c1, c0
    orr r0, #0x00000001
    bic r0, #0x00800000
    mcr p15, 0, r0, c1, c0
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 4
    bx lr

paging_invalidate_tlb:
    mov r0, #0
    mcr p15, 0, r0, c8, c7, 0
    bx lr

paging_invalidate_tlb_entry:
    mcr p15, 0, r0, c8, c7, 1
    bx lr

paging_activate_transition_table:
    mcr p15, 0, r0, c2, c0, 0
    bx lr
