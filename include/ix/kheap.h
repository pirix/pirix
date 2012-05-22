#pragma once

typedef unsigned size_t;

typedef struct heap_state {
    int valid;
    unsigned* vmem_mark;
    size_t used_bytes;
    size_t block_count;
} heap_state;

typedef struct heap {
    unsigned* start;
    unsigned* end;
    heap_state state;
} heap;

void kheap_init();
void* kmalloc(size_t size);
void kfree(void* addr);

extern heap kheap;
