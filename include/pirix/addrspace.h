#pragma once

typedef struct addrspace {
    uintptr_t pagedir;
    memarea* areas;
} addrspace;

addrspace* addrspace_new();
void addrspace_add_area(memarea* area);
void addrspace_delete(addrspace* as);
