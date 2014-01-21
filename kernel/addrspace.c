#include <pirix/addrspace.h>
#include <pirix/memarea.h>
#include <pirix/string.h>
#include <pirix/frame.h>
#include <pirix/kheap.h>

addrspace* addrspace_new() {
    addrspace* self = kmalloc(sizeof(addrspace));
    self->pagedir = paging_create_pagedir();
    self->areas = 0;
    return self;
}

void addrspace_add_area(addrspace* self, memarea* area) {
    if (area->backend->open) {
        area->backend->open(area);
    }

    if (!self->areas) {
        area->next = 0;
        self->areas = area;
    }
    else {
        memarea* curr = self->areas;

        while (curr && curr->start > area->start) {
            curr = curr->next;
        }

        area->next = curr->next;
        curr->next = area;
    }
}

uintptr_t addrspace_sbrk(addrspace* self, int incr) {
    if (incr == 0) {
        return self->heap.start + self->heap.size;
    }

    while (incr > self->heap.size - self->heap.used) {
       	uintptr_t virt = self->heap.start + self->heap.size;
        paging_map(virt, frame_alloc(), PAGE_PERM_USER);
        memset((void*)virt, 0, 0x1000);
        self->heap.size += 0x1000;
    }

    self->heap.used += incr;
    return self->heap.start;
}

void addrspace_pagefault(addrspace* self, uintptr_t addr, pf_type fault) {
    memarea* area = memarea_find(self->areas, addr);
    pf_status status = PF_FAULT;
    if (area) {
        status = area->backend->pagefault(area, addr, fault);
    }

    if (status != PF_RESOLVED) {
        panic("unresolvable page fault");
    }
}

void addrspace_delete(addrspace* self) {
    kfree(self, sizeof(addrspace));
}
