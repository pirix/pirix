#include <ix/kheap.h>
#include <ix/memory.h>
#include <ix/paging.h>
#include <string.h>

heap kheap;

void kheap_init() {
    memset(&kheap, 0, sizeof(heap));
    kheap.start = 0;
    kheap.end = 0;

    kheap.state.used_bytes = 0;
    kheap.state.block_count = 0;
    kheap.state.vmem_mark = kheap.start + 4096;

    unsigned phys = memory_alloc();
    unsigned virt = paging_map_kernel(phys);

    kheap.state.valid = 1;
}

void* kmalloc(size_t size) {
    return 0;
}

void kfree(void* addr) {

}
