#include <kernel/kheap.h>
#include <kernel/memory.h>
#include <kernel/paging.h>
#include <string.h>

#define BUCKET_COUNT 16

typedef struct kheap_block {
    struct kheap_block* next;
} kheap_block;

static kheap_block* kheap_bucket[BUCKET_COUNT];

static int kheap_bucket_find(size_t size) {
    int bucket = BUCKET_COUNT;

    for (int i = 0; i < BUCKET_COUNT; i++) {
        if (size <= WORD_SIZE*(1 << i)) {
            bucket = i;
            break;
        }
    }

    return bucket;
}

static void kheap_slab_create(int bucket) {
    unsigned phys = memory_alloc();
    unsigned virt = paging_map_kernel(phys);
    unsigned* slab = (unsigned*)virt;

    for (unsigned i = 0; i < 4096/WORD_SIZE; i += (1 << bucket)) {
        kfree(&slab[i], WORD_SIZE*(1<<bucket));
    }
}

void* kmalloc(size_t size) {
    int bucket = kheap_bucket_find(size);

    if (bucket > BUCKET_COUNT) return NULL;

    if (!kheap_bucket[bucket]) {
        kheap_slab_create(bucket);
    }

    kheap_block* block = kheap_bucket[bucket];
    kheap_bucket[bucket] = block->next;

    return block;
}

void* kcalloc(int count, size_t size) {
    void* ptr = kmalloc(size);
    memset(ptr, 0, size*count);
    return ptr;
}

void kfree(void* addr, size_t size) {
    int bucket = kheap_bucket_find(size);

    if (bucket > BUCKET_COUNT) return;

    kheap_block* block = addr;

    block->next = kheap_bucket[bucket];
    kheap_bucket[bucket] = block;
}
