#include <pirix/kheap.h>
#include <pirix/memarea.h>

memarea* memarea_new(const membackend* backend, int flags) {
    memarea* area = kmalloc(sizeof(memarea));
    area->backend = backend;
    area->flags = flags;
    return area;
}

memarea* memarea_find(memarea* first, uintptr_t addr) {
    memarea* curr = first;

    while (curr) {
        if (addr >= curr->start && addr <= curr->end) {
            return curr;
        }
        curr = curr->next;
    }

    return 0;
}

void memarea_delete(memarea* area) {
    kfree(area, sizeof(memarea));
}
