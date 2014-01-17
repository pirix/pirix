#include <pirix/addrspace.h>
#include <pirix/memarea.h>
#include <pirix/string.h>
#include <pirix/frame.h>
#include <pirix/kheap.h>

addrspace* addrspace_new() {
    addrspace* self = kmalloc(sizeof(addrspace));
    return self;
}

void addrspace_add_area(addrspace* self, memarea* area) {
    memarea* curr = self->areas;
    while (curr && curr->start > area->start) {
        curr = curr->next;
    }
    // TODO
}

uintptr_t addrspace_sbrk(addrspace* self, int incr) {
    if (incr == 0) {
        return self->heap.start + self->heap.size;
    }

    while (incr > self->heap.size - self->heap.used) {
       	uintptr_t virt = self->heap.start + self->heap.size;
        paging_map(self->pagedir, virt, frame_alloc(), PAGE_PERM_USER);
        memset((void*)virt, 0, 0x1000);
        self->heap.size += 0x1000;
    }

    self->heap.used += incr;
    return self->heap.start;
}

void addrspace_delete(addrspace* self) {
    kfree(self, sizeof(addrspace));
}
