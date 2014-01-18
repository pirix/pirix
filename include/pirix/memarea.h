#pragma once

#include <pirix/types.h>

typedef struct membackend {
    const char* name;
    void (*pagefault)(uintptr_t addr);
} membackend;

typedef struct memarea {
    uintptr_t start;
    uintptr_t end;
    int flags;
    struct membackend* backend;
    struct memarea* next;
} memarea;

#define MEM_READ     (1 << 0)
#define MEM_WRITE    (1 << 1)
#define MEM_EXEC     (1 << 2)
#define MEM_GROWUP   (1 << 3)
#define MEM_GROWDOWN (1 << 4)

/**
 * Create a new memarea.
 * @param backend The backend to use.
 * @memberof memarea
 */
memarea* memarea_new(membackend* backend, int flags);

/**
 * Find the corresponding memarea for an memory address.
 * @param first The memarea to start searching with.
 * @param addr The address to search for.
 * @memberof memarea
 */
memarea* memarea_find(memarea* first, uintptr_t addr);

/**
 * Delete the memarea.
 * @memberof memarea
 */
void memarea_delete(memarea* area);
