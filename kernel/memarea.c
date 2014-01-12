#include <pirix/kheap.h>
#include <pirix/memarea.h>

memarea* memarea_new(membackend* backend) {
    memarea* area = kmalloc(sizeof(memarea));
    area->backend = backend;
    return area;
}

void memarea_use(memarea* area) {
    area->refcount++;
}

void memarea_release(memarea* area) {
    area->refcount--;
    if (area->refcount <= 0) {
        memarea_free(area);
    }
}

void memarea_delete(memarea* area) {
    kfree(area, sizeof(memarea));
}
