#include <ix/kheap.h>
#include <ix/memory.h>
#include <ix/paging.h>

void* kmalloc(size_t size) {
    return paging_map_kernel((unsigned)memory_alloc());
}

void kfree(void* addr) {

}
