#pragma once

#include <pirix/types.h>

typedef struct membackend {
    const char* name;
    void (*pagefault)(uintptr_t addr);
} membackend;

typedef struct memarea {
    uintptr_t addr;
    size_t size;
    int flags;
    int refcount;
    membackend* backend;
    memarea* next;
} memarea;

/**
 * Create a new memarea.
 * @param backend The backend to use.
 * @memberof memarea
 */
memarea* memarea_new(membackend* backend);

/**
 * Increase the reference count of the memarea.
 * @memberof memarea
 */
void memarea_use(memarea* area);

/**
 * Decrease the reference count of the memarea.
 * Deletes the memarea if the reference count reaches zero.
 * @memberof memarea
 */
void memarea_release(memarea* area);

/**
 * Delete the memarea.
 * @memberof memarea
 */
void memarea_delete(memarea* area);
